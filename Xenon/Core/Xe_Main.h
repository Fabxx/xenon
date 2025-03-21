// Copyright 2025 Xenon Emulator Project

#pragma once
#include "Base/Config.h"
#include "Base/Logging/Backend.h"
#include "Base/Logging/Log.h"
#include "Base/Path_util.h"

#include "Core/NAND/NAND.h"
#include "Core/RAM/RAM.h"
#include "Core/RootBus/HostBridge/HostBridge.h"
#include "Core/RootBus/HostBridge/PCIBridge/AUDIOCTRLLR/AudioController.h"
#include "Core/RootBus/HostBridge/PCIBridge/EHCI0/EHCI0.h"
#include "Core/RootBus/HostBridge/PCIBridge/EHCI1/EHCI1.h"
#include "Core/RootBus/HostBridge/PCIBridge/ETHERNET/Ethernet.h"
#include "Core/RootBus/HostBridge/PCIBridge/HDD/HDD.h"
#include "Core/RootBus/HostBridge/PCIBridge/ODD/ODD.h"
#include "Core/RootBus/HostBridge/PCIBridge/OHCI0/OHCI0.h"
#include "Core/RootBus/HostBridge/PCIBridge/OHCI1/OHCI1.h"
#include "Core/RootBus/HostBridge/PCIBridge/PCIBridge.h"
#include "Core/RootBus/HostBridge/PCIBridge/SFCX/SFCX.h"
#include "Core/RootBus/HostBridge/PCIBridge/SMC/SMC.h"
#include "Core/RootBus/HostBridge/PCIBridge/XMA/XMA.h"
#include "Core/RootBus/RootBus.h"
#include "Core/XCPU/Xenon.h"
#include "Core/XGPU/XGPU.h"

#include "Render/Renderer.h"

class XeMain {
public:
  XeMain();
  ~XeMain();

  void start();

  void addPCIDevices();
  void createHostBridge();
  void createPCIDevices();
  void createRootBus();
  void createSMCState();

  void getFuses();

  bool isRunning() {
    return running;
  }

  void setRunning() {
    running = true;
  }
  void shutdown() {
    running = false;
  }

  Xenon* getCPU() {
    return xenonCPU.get();
  }
private:
  // Main objects
  //  Thread state
  bool running = true;
  //  Base path
  std::filesystem::path userDirectory;
  //  Log level
  std::unique_ptr<Base::Log::Filter> logFilter;

  // Main Emulator objects
  std::unique_ptr<RootBus> rootBus; // RootBus Object
  std::unique_ptr<HostBridge> hostBridge; // HostBridge Object
  std::unique_ptr<PCIBridge> pciBridge; // PCIBridge Object

public:
  // Render thread
  std::shared_ptr<Render::Renderer> renderer;

  // PCI Devices
  //  SMC
  std::shared_ptr<Xe::PCIDev::SMC::SMC_CORE_STATE> smcCoreState; // SMCCore State for setting diffrent SMC settings.
  std::unique_ptr<Xe::PCIDev::SMC::SMCCore> smcCore; // SMCCore Object
  //  Ethernet
  std::unique_ptr<Xe::PCIDev::ETHERNET::ETHERNET> ethernet;
  //  Audio
  std::unique_ptr<Xe::PCIDev::AUDIOCTRLR::AUDIOCTRLR> audioController;
  //  OHCI
  std::unique_ptr<Xe::PCIDev::OHCI0::OHCI0> ohci0;
  std::unique_ptr<Xe::PCIDev::OHCI1::OHCI1> ohci1;
  //  EHCI
  std::unique_ptr<Xe::PCIDev::EHCI0::EHCI0> ehci0;
  std::unique_ptr<Xe::PCIDev::EHCI1::EHCI1> ehci1;
  //  Secure Flash Controller for Xbox Device object
  std::shared_ptr<SFCX> sfcx;
  //  NAND
  std::shared_ptr<NAND> nandDevice;
  //  Random Access Memory (All console RAM, excluding Reserved memory which is mainly PCI Devices)
  std::shared_ptr<RAM> ram;
  //  XMA
  std::shared_ptr<XMA> xma;
  //  ODD (CD-ROM Drive)
  std::shared_ptr<ODD> odd;
  //  HDD
  std::shared_ptr<HDD> hdd;

private:
  // Console Handles
  //  Xenon CPU
  std::shared_ptr<Xenon> xenonCPU;
  //  Xenos GPU
  std::shared_ptr<Xe::Xenos::XGPU> xenos;
  //  Fuses
  eFuses cpuFuses;
};

inline std::unique_ptr<XeMain> Xe_Main{};