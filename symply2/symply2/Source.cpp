#include <ntddk.h> 
#include <wdm.h>

//#define PRIORITY_BOOSTER_DEVICE 0x8000
//#define IOCTL_PRIORITY_BOOSTER_SET_PRIORITY CTL_CODE(PRIORITY_BOOSTER_DEVICE, \
// 0x800, METHOD_NEITHER, FILE_ANY_ACCESS)

extern "C"
NTSTATUS OnCreate(PDEVICE_OBJECT DeviceObject, PIRP irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	KdPrint(("Sample driver OnCreate called\n"));
	return STATUS_SUCCESS;
}

extern "C"
NTSTATUS OnClose(PDEVICE_OBJECT DeviceObject, PIRP irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	KdPrint(("Sample driver OnClose called\n"));
	return STATUS_SUCCESS;
}

extern "C"
UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\EXAMPLE");

void OnDrvUnload(_In_ PDRIVER_OBJECT DriverObject) {
	UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\EXAMPLE");
	IoDeleteSymbolicLink(&symLink);
	IoDeleteDevice(DriverObject->DeviceObject);
	KdPrint(("Sample driver Unload called\n"));
}
extern "C"
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath) {
	KdPrint(("Sample driver Load called\n"));
	DriverObject->MajorFunction[IRP_MJ_CREATE] = OnCreate;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = OnClose;
	DriverObject->DriverUnload = OnDrvUnload;
	NTSTATUS status;
	PDEVICE_OBJECT DeviceObject = NULL;
	status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);
	if (!NT_SUCCESS(status)) {
		KdPrint(("Error: 0x%08X\n", status));
		return status;
	}

	UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\EXAMPLE");
	status = IoCreateSymbolicLink(&symLink, &DeviceName);
	if (!NT_SUCCESS(status)) {
		KdPrint(("Failed to create symbolic link (0x%08X)\n", status));
		IoDeleteDevice(DeviceObject);
		return status;
	}

	UNREFERENCED_PARAMETER(RegistryPath);
	
	return STATUS_SUCCESS;
}

