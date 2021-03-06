#ifndef _SDK_CHLCLIENT_
#define _SDK_CHLCLIENT_
#pragma once

#define FFADE_IN			0x0001		// Just here so we don't pass 0 into the function
#define FFADE_OUT			0x0002		// Fade out (not in)
#define FFADE_MODULATE		0x0004		// Modulate (don't blend)
#define FFADE_STAYOUT		0x0008		// ignores the duration, stays faded out until new ScreenFade message received
#define FFADE_PURGE			0x0010		// Purges all other fades, replacing them with this one

#define SCREENFADE_FRACBITS		9		// which leaves 16-this for the integer part

// This structure is sent over the net to describe a screen fade event
struct ScreenFade_t{
	unsigned short 	duration;		// FIXED 16 bit, with SCREENFADE_FRACBITS fractional, seconds duration
	unsigned short 	holdTime;		// FIXED 16 bit, with SCREENFADE_FRACBITS fractional, seconds duration until reset (fade & hold)
	short			fadeFlags;		// flags
	unsigned char	r, g, b, a;		// fade to color ( max alpha )
};

class IFileList;
class CEngineSprite;
class CViewSetup;
class bf_write;
class bf_read;
class CRenamedRecvTableInfo;
class CStandardRecvProxies;
class ClientClass;
class CGlobalVarsBase;
class CSaveRestoreData;
class QAngle;
class CMouthInfo;

enum ButtonCode_t;
enum ClientFrameStage_t;

struct datamap_t;
struct typedescription_t;
struct vrect_t;

class CHLClient {
public:
	virtual int						Init(void* appSystemFactory, void* physicsFactory, CGlobalVarsBase* pGlobals) = 0;
	virtual void					PostInit() = 0;
	virtual void					Shutdown(void) = 0;
	virtual bool					ReplayInit(void* fnReplayFactory) = 0;
	virtual bool					ReplayPostInit() = 0;
	virtual void					LevelInitPreEntity(const char* pMapName) = 0;
	virtual void					LevelInitPostEntity() = 0;
	virtual void					LevelShutdown(void) = 0;
	virtual ClientClass*			GetAllClasses(void) = 0;
	virtual int						HudVidInit(void) = 0;
	virtual void					HudProcessInput(bool bActive) = 0;
	virtual void					HudUpdate(bool bActive) = 0;
	virtual void					HudReset(void) = 0;
	virtual void					HudText(const char* message) = 0;
	virtual void					IN_ActivateMouse(void) = 0;
	virtual void					IN_DeactivateMouse(void) = 0;
	virtual void					IN_Accumulate(void) = 0;
	virtual void					IN_ClearStates(void) = 0;
	virtual bool					IN_IsKeyDown(const char* name, bool& isdown) = 0;
	virtual void					IN_OnMouseWheeled(int nDelta) = 0;
	virtual int						IN_KeyEvent(int eventcode, ButtonCode_t keynum, const char* pszCurrentBinding) = 0;
	virtual void					CreateMove(int sequence_number, float input_sample_frametime, bool active) = 0;
	virtual void					ExtraMouseSample(float frametime, bool active) = 0;
	virtual bool					WriteUsercmdDeltaToBuffer(bf_write* buf, int from, int to, bool isnewcommand) = 0;
	virtual void					EncodeUserCmdToBuffer(bf_write& buf, int slot) = 0;
	virtual void					DecodeUserCmdFromBuffer(bf_read& buf, int slot) = 0;
	virtual void					View_Render(vrect_t* rect) = 0;
	virtual void					RenderView(const CViewSetup& view, int nClearFlags, int whatToDraw) = 0;
	virtual void					View_Fade(ScreenFade_t* pSF) = 0;
	virtual void					SetCrosshairAngle(const QAngle& angle) = 0;
	virtual void					InitSprite(CEngineSprite* pSprite, const char* loadname);
	virtual void					ShutdownSprite(CEngineSprite* pSprite);
	virtual int						GetSpriteSize(void) const;
	virtual void					VoiceStatus(int entindex, int bTalking);
	virtual void					InstallStringTableCallback(const char* tableName);
	virtual void					FrameStageNotify(ClientFrameStage_t curStage);
	virtual bool					DispatchUserMessage(int msg_type, bf_read& msg_data);
	virtual CSaveRestoreData*		SaveInit(int size);
	virtual void					SaveWriteFields(CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int);
	virtual void					SaveReadFields(CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int);
	virtual void					PreSave(CSaveRestoreData*);
	virtual void					Save(CSaveRestoreData*);
	virtual void					WriteSaveHeaders(CSaveRestoreData*);
	virtual void					ReadRestoreHeaders(CSaveRestoreData*);
	virtual void					Restore(CSaveRestoreData*, bool);
	virtual void					DispatchOnRestore();
	virtual CStandardRecvProxies*	GetStandardRecvProxies();
	virtual void					WriteSaveGameScreenshot(const char* pFilename);
	virtual void					EmitSentenceCloseCaption(char const* tokenstream);
	virtual void					EmitCloseCaption(char const* captionname, float duration);
	virtual bool					CanRecordDemo(char* errorMsg, int length) const;
	virtual void					OnDemoRecordStart(char const* pDemoBaseName);
	virtual void					OnDemoRecordStop();
	virtual void					OnDemoPlaybackStart(char const* pDemoBaseName);
	virtual void					OnDemoPlaybackStop();
	virtual bool					ShouldDrawDropdownConsole();
	virtual int						GetScreenWidth(); // 56
	virtual int						GetScreenHeight();
	virtual void					WriteSaveGameScreenshotOfSize(const char* pFilename, int width, int height, bool bCreatePowerOf2Padded/*=false*/, bool bWriteVTF/*=false*/);
	virtual bool					GetPlayerView(CViewSetup& playerView);
	virtual void					SetupGameProperties(void* contexts, void* properties);
	virtual unsigned int			GetPresenceID(const char* pIDName);
	virtual const char*				GetPropertyIdString(const unsigned int id);
	virtual void					GetPropertyDisplayString(unsigned int id, unsigned int value, char* pOutput, int nBytes);
	virtual void					InvalidateMdlCache();
	virtual void					IN_SetSampleTime(float frametime);
	virtual void					ReloadFilesInList(IFileList* pFilesToReload);
	virtual void					StartStatsReporting(void* handle, bool bArbitrated);
	virtual bool					HandleUiToggle();
	virtual bool					ShouldAllowConsole();
	virtual CRenamedRecvTableInfo*	GetRenamedRecvTableInfos();
	virtual CMouthInfo*				GetClientUIMouthInfo();
	virtual void					FileReceived(const char* fileName, unsigned int transferID);
	virtual const char*				TranslateEffectForVisionFilter(const char* pchEffectType, const char* pchEffectName);


	// there's more shit below 
};

#endif