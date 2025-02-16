#include "qemu/osdep.h"
#include "qemu/module.h"
#include "qemu/typedefs.h"
#include "qom/object.h"
#include "qemu/units.h"
#include "hw/pci/pci_device.h"

#define TYPE_PCI_GPU_DEVICE "gpu"
#define GPU_DEVICE_ID 0x1337

struct GpuState {
  PCIDevice pdev;
  // memory region
  MemoryRegion mem;
  // 1MB array of chars, used as memory
  unsigned char data[0x100000];
};
typedef struct GpuState GpuState;

// This creates GPU type cast from PCIDevice, probably more helper stuff
DECLARE_INSTANCE_CHECKER(GpuState, GPU, TYPE_PCI_GPU_DEVICE);

static void gpu_instance_init(Object *obj) { printf("GPU instance init\n"); }

static uint64_t gpu_mem_read(void *opaque, hwaddr addr, unsigned size) {
  GpuState *gpu = opaque;
  // Convert size from bytes to bits
  uint64_t bitcount = ((uint16_t)size) << 3;
  // 1ULL is unsigned long long (64 bit integer)
  // This line is just getting a mask of 1s for the read size
  uint64_t mask = (1ULL << bitcount) - 1;
  // Now read using mask
  uint64_t got = gpu->data[addr] & mask;
  printf("Tried to read 0x%x bytes at 0x%llx = 0x%llx\n", size, addr, got);
  return got;
}
static void gpu_mem_write(void *opaque, hwaddr addr, uint64_t val,
                          unsigned size) {
  GpuState *gpu     = opaque;
  uint64_t bitcount = ((uint16_t)size) << 3;
  uint64_t mask     = (1ULL << bitcount) - 1;
  // Truncate val to size
  uint64_t sizedval = val & mask;
  gpu->data[addr]   = sizedval;
  printf("Tried to write 0x%llx [0x%llx] (0x%x bytes) at 0x%llx\n", val,
         sizedval, size, addr);
}
static const MemoryRegionOps gpu_mem_ops = {
    .read  = gpu_mem_read,
    .write = gpu_mem_write,
};

static void pci_gpu_realize(PCIDevice *pdev, Error **errp) {
  printf("GPU Realize\n");
  // GPU is a typecast (see above DECLARE_INSTANCE_CHECKER call)
  GpuState *gpu = GPU(pdev);
  memory_region_init_io(&gpu->mem, OBJECT(gpu), &gpu_mem_ops, gpu, "gpu-mem",
                        1 * MiB);
  // configure a base address register to contain base address and size
  // Each BAR is a 32-bit location that points to another memory region
  // Each BAR tells the CPU the base address, width, and other properties of
  // this region.
  // This should show up in lspci -vv
  pci_register_bar(
      pdev, 0,                       // BAR number
      PCI_BASE_ADDRESS_SPACE_MEMORY, // specify this is a memory space BAR
      &gpu->mem                      // memory region to map
  );
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
