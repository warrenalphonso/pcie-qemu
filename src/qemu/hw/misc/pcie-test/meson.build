pcie_test_ss = ss.source_set()
pcie_test_ss.add(files(
    'gpu.c',
))

system_ss.add_all(when: 'CONFIG_PCIE_TEST', if_true: pcie_test_ss)
