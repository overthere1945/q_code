zephyr_interface_library_named(qurt_abstraction_layer)

target_include_directories(qurt_abstraction_layer INTERFACE inc)

zephyr_library()
zephyr_library_sources_ifdef(CONFIG_QURT_THREAD src/qurt_thread.c)
zephyr_library_sources_ifdef(CONFIG_QURT_MUTEX src/qurt_mutex.c)
zephyr_library_sources_ifdef(CONFIG_QURT_ISR src/qurt_isr.c)
zephyr_library_sources_ifdef(CONFIG_QURT_SIGNAL src/qurt_signal.c)
zephyr_library_sources_ifdef(CONFIG_QURT_SEMAPHORE src/qurt_sem.c)
zephyr_library_sources_ifdef(CONFIG_QURT_MEM src/qurt_mem.c)

zephyr_library_include_directories(
  ${ZEPHYR_BASE}/kernel/include
  ${ARCH_DIR}/${ARCH}/include
)

zephyr_library_link_libraries(qurt_abstraction_layer)
zephyr_library_property(ALLOW_EMPTY TRUE)