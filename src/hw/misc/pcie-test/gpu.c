#include "qemu/osdep.h"
#include "qemu/module.h"
#include "qemu/typedefs.h"
#include "qom/object.h"
#include "hw/pci/pci_device.h"

#define TYPE_PCI_GPU_DEVICE "gpu"
#define GPU_DEVICE_ID 0x1337

struct GpuState {
  PCIDevice pdev;
};
typedef struct GpuState GpuState;

static void gpu_instance_init(Object *obj) { printf("GPU instance init\n"); }

static void pci_gpu_realize(PCIDevice *pdev, Error **errp) {
  printf("GPU Realize\n");
}

static void pci_gpu_uninit(PCIDevice *pdev) { printf("GPU un-init\n"); }

static void gpu_class_init(ObjectClass *class, void *data) {
  printf("Class init\n");
  PCIDeviceClass *k = PCI_DEVICE_CLASS(class);

  k->realize   = pci_gpu_realize;
  k->exit      = pci_gpu_uninit;
  k->vendor_id = PCI_VENDOR_ID_QEMU;
  k->device_id = GPU_DEVICE_ID;
  k->class_id  = PCI_CLASS_OTHERS;
}

static void pci_gpu_register_types(void) {
  static InterfaceInfo interfaces[] = {
      // macro that means device behaves like a standard PCI device
      {INTERFACE_CONVENTIONAL_PCI_DEVICE},
      // indicates array is over
      {},
  };

  static const TypeInfo gpu_info = {
      .name          = TYPE_PCI_GPU_DEVICE,
      .parent        = TYPE_PCI_DEVICE,
      .instance_size = sizeof(GpuState),
      .instance_init = gpu_instance_init,
      .class_init    = gpu_class_init,
      .interfaces    = interfaces,
  };

  type_register_static(&gpu_info);
}

// tell QEMU object model about this device
// https://qemu-project.gitlab.io/qemu/devel/qom.html
type_init(pci_gpu_register_types);
