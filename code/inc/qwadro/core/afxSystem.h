/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group � Federa��o SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_SYSTEM_H
#define AFX_SYSTEM_H

#include "qwadro/core/afxKeyboard.h"
#include "qwadro/core/afxMouse.h"
#include "qwadro/core/afxStream.h"
#include "qwadro/core/afxUri.h"
#include "qwadro/core/afxArena.h"
#include "qwadro/core/afxTime.h"
#include "qwadro/core/afxModule.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/core/afxFile.h"
#include "qwadro/core/afxResource.h"
#include "qwadro/core/afxArchive.h"
#include "qwadro/core/afxTxu.h"
#include "qwadro/core/afxPool.h"
#include "qwadro/core/afxMap.h"

enum // opcodes used for primitive communication bethween engine and modules.
{
    AFX_OPCODE_CONTINUE,
    AFX_OPCODE_SUSPEND,
    AFX_OPCODE_BREAK, // if system returns BREAK, the engine must shutdown and exit to operating system.
    AFX_OPCODE_REBOOT, // if system returns REBOOT, the engine must shutdown then reinitialize the system.

    AFX_OPCODE_DETACH = 0, // informa a um m�dulo que ele ser� desacoplado do sistema, dando-lhe uma chance de liberar recursos.
    AFX_OPCODE_ATTACH, // informa a um m�dulo que ele ser� acoplado ao sistema, dando-lhe uma chance de realizar sua devida initializa��o de recursos e opera��es.
};

#if 0
A implenta��o atual de sistema de arquivos � mentalizada de forma simples.
A opera��o requerida � tentada para cada volume montado, volume este de prefixo correspondente,
entretando, come�ando a partir do mais recentemente mountado.Um prefixo vazio � considerado
a sempre corresponder, assim frequentemente ser�o usados como um 'fallback' final.

Uma configura��o t�pica para um dispositivo m�vel deveria prover muitos volumes, dentre - os:
um volume somente - leitura para o pacote da aplica��o, ent�o um volume modifici�vel para o
uma localiza��o dentro do mmcard.E finalmente um volume capture - tudo para manejar caminhos
absolutos.

1. ("Resources", "/path/to/app/bundle/Resources", writable = False)
2. ("Resources", "/mnt/sdcard/Havok/Resources", writable = True)
3. ("", "", writable = False)

There is no support for whiteouts so if a file exists in more than
one mount, it will still be present after removing the first one.
Similarly when iterating, a file with the same name but different
contents may be returned.
#endif

typedef struct afxResourceInfo
{
    // virtual file
    // blob
    // file
    // directory
    afxBool isFile; // else it is a directory

} afxResourceInfo;

#define AFX_FS_SYM_LEN  (32)
#define AFX_FS_DIR_LEN  (1024 + 1 + AFX_FS_SYM_LEN + 1)

// A mount point is a directory on a file system that is logically linked to another file system.
// Mount points are used to make the data on a different physical storage drive easily available in a folder structure.
// Mount points are fundamental to Unix, Linux and macOS. Windows can use mount points, but it is not common.

// Mounting is a process by which the operating system makes files and directories on a storage device available for users to access via the computer's file system.

AFX_DEFINE_STRUCT(afxStoragePointSpecification)
{
    afxUri const    *namespace; // exchange point
    afxUri const    *hostPath;
    afxIoFlags      perm;
    afxBool         isZip;
};

AFX_DEFINE_STRUCT(afxFileInfo)
{
    afxFileFlags perm;
    afxNat      devId; // Device ID of device containing file.
    afxNat16    inodeNum; // File serial number. Aka inode number (unused on Windows)
    afxNat16    mode; // Mode of file
    afxInt16    nofLinks; // Number of hard links to the file.
    afxInt16    uid; // User ID of file.
    afxInt16    gid; // Group ID of file.
    afxNat      rdevId; // Device ID (if file is character or block special).
    afxSize     siz; // For regular files, the file size in bytes. For symbolic links, the length in bytes of the pathname contained in the symbolic link. For a shared memory object, the length in bytes. For a typed memory object, the length in bytes. For other file types, the use of this field is unspecified.
    afxTime     lastAccess; // Time of last access.
    afxTime     lastMod; // Time of last data modification.
    afxTime     creationTime; // Time of last status change.
};

AFX_CALLBACK(void, afxProfilerPushTimerFunc)(aaxPluginId in_uPluginID, const char* in_pszZoneName);
AFX_CALLBACK(void, afxProfilerPopTimerFunc)(void);
AFX_CALLBACK(void, afxProfilerPostMarkerFunc)(aaxPluginId in_uPluginID, const char* in_pszMarkerName );

AFX_DEFINE_STRUCT(afxSystemConfig)
{
    afxChar const*              root;
    afxSize                     maxMemUsage;
    afxNat                      memPageSiz;
    afxNat                      allocGranularity;
    afxNat                      genrlArenaSpace;

    afxNat                      ioBufSiz;
    afxNat                      ioArenaSpace;
    afxAssertHook               assertHook; // external assertion handling function (optional)
    afxNat                      maxHwThreads; // max amount of hardware threads managed by Qwadro.
    afxReal                     unitsToMeter; // the number of units in a meter.
    
    afxNat                      handlePageSiz;

    afxProfilerPushTimerFunc    profilerPushTimer; // external (optional) function for tracking performance of the system that is called when a timer starts. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPopTimerFunc     profilerPopTimer; // external (optional) function for tracking performance of the system that is called when a timer stops. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPostMarkerFunc   profilerPostMarker; // external (optional) function for tracking significant events in the system, to act as a marker or bookmark. (only called in Debug and Profile binaries; this is not called in Release)

    afxSize const*              attrs[2];
    
    afxDrawSystemConfig const*  dsys;
    void const*                 ssys;
};

AFX_DEFINE_STRUCT(afxSystemConfigWin32)
{
    afxSystemConfig             base;
#ifdef AFX_PLATFORM_WIN32
    /*HINSTANCE*/void*          instance;
    /*HWND*/void*               hWnd;
#endif
};

#ifdef _AFX_CORE_C
#ifdef _AFX_SYSTEM_C
AFX_OBJECT(afxSystem)
{
    afxUri                      rootDir;

    afxChain                    classes;
    afxClass                    mmus;
    afxClass                    modules;
    afxClass                    keyboards;
    afxClass                    mouses;
    afxClass                    threads;
    afxClass                    txus;
    afxClass                    services;
    afxClass                    applications;
    afxClass                    streams;
    afxClass                    files;
    afxClass                    archives;
    afxClass                    simulations;

    afxMmu                      mmu;
    afxNat                      memPageSize; // The page size and the granularity of page protection and commitment.
    afxNat                      allocGranularity;
    afxNat                      hwConcurrencyCap; // # of logical proc units (hardware threads)
    
    afxModule                   e2coree;
    afxKeyboard                 stdKbd;

    afxSize                     maxMemUsage;
    
    afxAssertHook               assertHook; // external assertion handling function (optional)
    afxReal                     unitsToMeter; // the number of units in a meter.

    afxProfilerPushTimerFunc    profilerPushTimer; // external (optional) function for tracking performance of the system that is called when a timer starts. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPopTimerFunc     profilerPopTimer; // external (optional) function for tracking performance of the system that is called when a timer stops. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPostMarkerFunc   profilerPostMarker; // external (optional) function for tracking significant events in the system, to act as a marker or bookmark. (only called in Debug and Profile binaries; this is not called in Release)

    afxBool                     isInBootUp;
    afxBool                     isInShutdown;
    afxBool                     operating;
    afxBool                     interruptionRequested;

    afxArena                    ioArena;
    afxNat                      ioBufSiz;
    afxChain                    mountPoints;
    afxChain                    aliveRsrcs;
    struct
    {
        struct
        {
            afxFcc              type;
            afxChain            resources;
        }                       supplyChain[1];
    }                           resourcing;
    afxInt                      exitCode;

    afxDrawSystem               dsys;
    afxSoundSystem              ssys;
};
#endif//_AFX_SYSTEM_C
#endif//_AFX_CORE_C

AFX void                AfxChooseBasicIoSystemConfiguration(afxSystemConfig *config, afxNat extendedSiz);
AFX afxError            AfxBootUpBasicIoSystem(afxSystemConfig const *config, afxSystem *sys);
AFX void                AfxShutdownBasicIoSystem(afxInt exitCode);
AFX afxBool             AfxGetSystem(afxSystem *sys);

AFX afxBool             AfxSystemIsExecuting(void);
AFX afxResult           AfxDoSystemThreading(afxTime timeout);
AFX void                AfxRequestSystemShutdown(afxInt exitCode);

AFX afxMmu          AfxGetIoContext(void);
AFX afxArena*           AfxGetIoArena(void);
AFX afxNat              AfxGetIoBufferSize(void);


AFX afxNat              AfxGetMemoryPageSize(void);

/// Returns the ideal number of threads that this process can run in parallel. 
/// This is done by querying the number of logical processors available to this process (if supported by this OS) or the total number of logical processors in the system. 
/// This function returns 1 if neither value could be determined.
AFX afxNat              AfxGetThreadingCapacity(void);

AFX afxMmu          AfxGetSystemContext(void);
AFX afxUri const*       AfxGetSystemRootPath(afxUri *copy);
AFX afxString const*    AfxGetSystemRootPathString(afxBufferedString *copy);

// Sends event event directly to receiver receiver, using the notify() function. Returns the value that was returned from the event handler.
AFX afxBool             AfxEmitEvent(afxInstance *receiver, afxEvent *ev);
AFX afxBool             AfxEmitEvents(afxNat cnt, afxInstance *receiver[], afxEvent ev[]);
AFX afxBool             AfxReemitEvent(afxNat cnt, afxInstance *receiver[], afxEvent *ev);

AFX afxError            AfxPostEvent(afxInstance *receiver, afxEvent *ev);
AFX afxError            AfxPostEvents(afxNat cnt, afxInstance *receiver[], afxEvent ev[]);

AFX afxNat              AfxGetStoragePointCount(void);
AFX afxResult           AfxMountStoragePoints(afxNat cnt, afxStoragePointSpecification const spec[]);
AFX afxResult           AfxDismountStoragePoints(afxNat cnt, afxStoragePointSpecification const spec[]);
AFX afxResult           AfxDescribeStoragePoints(afxNat first, afxNat cnt, afxStoragePointSpecification spec[]);

AFX afxError            AfxEnableResourceMonitoring(afxResource res, afxBool enable);

AFX afxError            AfxResolveUri(afxUri const *in, afxFileFlags permissions, afxUri *out);
AFX afxError            AfxResolveUris(afxNat cnt, afxUri const in[], afxFileFlags const permissions[], afxUri out[]);


AFX afxClass*           AfxGetApplicationClass(void);
AFX afxClass*           AfxGetArchiveClass(void);
AFX afxClass*           AfxGetMemoryManagementUnitClass(void);
AFX afxClass*           AfxGetFileClass(void);
AFX afxClass*           AfxGetKeyboardClass(void);
AFX afxClass*           AfxGetMouseClass(void);
AFX afxClass*           AfxGetModuleClass(void);
AFX afxClass*           AfxGetServiceClass(void);
AFX afxClass*           AfxGetSimulationClass(void);
AFX afxClass*           AfxGetStreamClass(void);
AFX afxClass*           AfxGetThreadClass(void);
AFX afxClass*           AfxGetThreadExecutionUnitClass(void);

AFX afxNat              AfxCountApplications(void);
AFX afxNat              AfxCountArchives(void);
AFX afxNat              AfxCountContexts(void);
AFX afxNat              AfxCountFiles(void);
AFX afxNat              AfxCountKeyboards(void);
AFX afxNat              AfxCountModules(void);
AFX afxNat              AfxCountMouses(void);
AFX afxNat              AfxCountServices(void);
AFX afxNat              AfxCountSimulations(void);
AFX afxNat              AfxCountStreams(void);
AFX afxNat              AfxCountThreads(void);

AFX afxNat              AfxEnumerateApplications(afxNat first, afxNat cnt, afxApplication app[]);
AFX afxNat              AfxEnumerateArchives(afxNat first, afxNat cnt, afxArchive arc[]);
AFX afxNat              AfxEnumerateContexts(afxNat first, afxNat cnt, afxMmu mmu[]);
AFX afxNat              AfxEnumerateFiles(afxNat first, afxNat cnt, afxFile file[]);
AFX afxNat              AfxEnumerateKeyboards(afxNat first, afxNat cnt, afxKeyboard kbd[]);
AFX afxNat              AfxEnumerateModules(afxNat first, afxNat cnt, afxModule mdle[]);
AFX afxNat              AfxEnumerateMouses(afxNat first, afxNat cnt, afxMouse mse[]);
AFX afxNat              AfxEnumerateServices(afxNat first, afxNat cnt, afxService svc[]);
AFX afxNat              AfxEnumerateSimulations(afxNat first, afxNat cnt, awxSimulation sim[]);
AFX afxNat              AfxEnumerateStreams(afxNat first, afxNat cnt, afxStream ios[]);
AFX afxNat              AfxEnumerateThreads(afxNat first, afxNat cnt, afxThread thr[]);

AFX afxNat              AfxCurateApplications(afxNat first, afxNat cnt, afxBool(*f)(afxApplication, void*), void *udd);
AFX afxNat              AfxCurateArchives(afxNat first, afxNat cnt, afxBool(*f)(afxArchive, void*), void *udd);
AFX afxNat              AfxCurateContexts(afxNat first, afxNat cnt, afxBool(*f)(afxMmu, void*), void *udd);
AFX afxNat              AfxCurateFiles(afxNat first, afxNat cnt, afxBool(*f)(afxFile, void*), void *udd);
AFX afxNat              AfxCurateKeyboards(afxNat first, afxNat cnt, afxBool(*f)(afxKeyboard, void*), void *udd);
AFX afxNat              AfxCurateModules(afxNat first, afxNat cnt, afxBool(*f)(afxModule, void*), void *udd);
AFX afxNat              AfxCurateMouses(afxNat first, afxNat cnt, afxBool(*f)(afxMouse, void*), void *udd);
AFX afxNat              AfxCurateServices(afxNat first, afxNat cnt, afxBool(*f)(afxService, void*), void *udd);
AFX afxNat              AfxCurateSimulations(afxNat first, afxNat cnt, afxBool(*f)(awxSimulation, void*), void *udd);
AFX afxNat              AfxCurateStreams(afxNat first, afxNat cnt, afxBool(*f)(afxStream, void*), void *udd);
AFX afxNat              AfxCurateThreads(afxNat first, afxNat cnt, afxBool(*f)(afxThread, void*), void *udd);


#if 0
////////////////////////////////////////////////////////////////////////////////
// QWADRO PLUGIN SYSTEM
////////////////////////////////////////////////////////////////////////////////

typedef void afxPlugin;
typedef void afxPluginParam;

typedef enum afxDeviceState
{
    afxDeviceState_Unknown = 0,         ///< The audio device state is unknown or invalid.
    afxDeviceState_Active = 1 << 0,     ///< The audio device is active That is, the audio adapter that connects to the endpoint device is present and enabled.
    afxDeviceState_Disabled = 1 << 1,   ///< The audio device is disabled.
    afxDeviceState_NotPresent = 1 << 2, ///< The audio device is not present because the audio adapter that connects to the endpoint device has been removed from the system.
    afxDeviceState_Unplugged = 1 << 3,  ///< The audio device is unplugged.
    afxDeviceState_All = afxDeviceState_Active | afxDeviceState_Disabled | afxDeviceState_NotPresent | afxDeviceState_Unplugged, ///< Includes audio devices in all states.
} afxDeviceState;

AFX_DEFINE_STRUCT(afxDeviceDescription)
{
    afxNat32 idDevice;                                          ///< Device ID for Wwise. This is the same as what is returned from AK::GetDeviceID and AK::GetDeviceIDFromName. Use it to specify the main device in AkPlatformInitSettings.idAudioDevice or in AK::SoundEngine::AddSecondaryOutput. 
    char deviceName[128];                           ///< The user-friendly name for the device.
    afxDeviceState deviceStateMask; ///< Bitmask used to filter the device based on their state.
    afxBool isDefaultDevice;                               ///< Identify default device. Always false when not supported.
};

/// Registered plugin creation function prototype.

AFX_CALLBACK(afxPlugin*, afxCreatePluginCallback)(afxAllocator * in_pAllocator);

// Registered plugin parameter node creation function prototype.

AFX_CALLBACK(afxPluginParam*, afxCreateParamCallback)(afxAllocator * in_pAllocator);

// Registered plugin device enumeration function prototype, used for providing lists of devices by plug-ins.

AFX_CALLBACK(afxResult, afxGetDeviceListCallback)
(
    afxNat32    *io_maxNumDevices,                 ///< In: The length of the out_deviceDescriptions array, or zero is out_deviceDescriptions is null. Out: If out_deviceDescriptions is not-null, this should be set to the number of entries in out_deviceDescriptions that was populated (and should be less-than-or-equal to the initial value). If out_deviceDescriptions is null, this should be set to the maximum number of devices that may be returned by this callback.
    afxDeviceDescription* out_deviceDescriptions ///< The output array of device descriptions. If this is not-null, there will be a number of entries equal to the input value of io_maxNumDevices.
);

/// Plug-in type.

typedef enum afxPluginType
{
    afxPluginTypeNone = 0,   ///< Unknown/invalid plug-in type.
    afxPluginTypeCodec = 1,  ///< Compressor/decompressor plug-in (allows support for custom audio file types).
    afxPluginTypeSource = 2, ///< Source plug-in: creates sound by synthesis method (no input, just output).
    afxPluginTypeEffect = 3, ///< Effect plug-in: applies processing to audio data.
    afxPluginTypeMixer = 6,  ///< Mixer plug-in: mix voices at the bus level.
    afxPluginTypeSink = 7,   ///< Sink plug-in: implement custom sound engine end point.
    afxPluginTypeGlobalExtension = 8,    ///< Global Extension plug-in: (e.g. Spatial Audio, Interactive Music)
    afxPluginTypeMetadata = 9,   ///< Metadata plug-in: applies object-based processing to audio data
    afxPluginTypeMask = 0xf  ///< Plug-in type mask is 4 bits.
} afxPluginType;

/// Registers a plug-in with the sound engine and sets the callback functions to create the plug-in and its parameter node.  
/// This function is deprecated. Registration is now automatic if you link plug-ins statically. If plug-ins are dynamic libraries (such as DLLs or SOs), use \c RegisterPluginDLL.
/// Codecs and plug-ins must be registered before loading banks that use them.
/// Loading a bank referencing an unregistered plug-in or codec will result in a load bank success, but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, posting the event will fail.

AFX_EXTERN_API_FUNC(afxError, RegisterPlugin)
(
    afxPluginType in_eType, // Plug-in type (for example, source or effect)
    afxNat32 in_ulCompanyID, // Company identifier (as declared in the plug-in description XML file)
    afxNat32 in_ulPluginID, // Plug-in identifier (as declared in the plug-in description XML file)
    afxCreatePluginCallback in_pCreateFunc, // Pointer to the plug-in's creation function
    afxCreateParamCallback in_pCreateParamFunc, // Pointer to the plug-in's parameter node creation function
    afxGetDeviceListCallback in_pGetDeviceList // Optional pointer to the plug-in's device enumeration function. Specify for a sink plug-in to support \ref AK::SoundEngine::GetDeviceList.
);



#endif

#endif//AFX_SYSTEM_H