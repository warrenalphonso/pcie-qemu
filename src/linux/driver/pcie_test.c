// Linux PCI driver
// https://elixir.bootlin.com/linux/v6.9/source/include/linux/pci.h#L887
#include <linux/pci.h>

static struct pci_device_id gpu_id_tbl[] = {
    {PCI_DEVICE(0x1234, 0x1337)},
    // Indicates array is terminated
    {0},
};
// Creates alias __mod_device_table__pci__gpu_id_tbl for gpu_id_tbl that kernel
// will pick up during build
MODULE_DEVICE_TABLE(pci, gpu_id_tbl);

MODULE_LICENSE("GPL");

static int gpu_probe(struct pci_dev *pdev, const struct pci_device_id *id) {
  pr_info("called probe");
  return 0;
}

static struct pci_driver gpu_driver = {
    .name     = "gpu",
    .id_table = gpu_id_tbl,
    .probe    = gpu_probe,
};

static struct class *gpu_class;
static int __init gpu_module_init(void) {
  int err;
  gpu_class = class_create("pcie_test");
  err       = pci_register_driver(&gpu_driver);
  pr_info("gpu_module_init done\n");
  return err;
}

module_init(gpu_module_init);