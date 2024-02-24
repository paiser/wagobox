//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Wago Modbus Tango Server
//  @ File Name : BufferHolder.h
//  @ Date : 2013-02-03
//  @ Author : Pawel Kowaliszyn
//
//


#if !defined(_BUFFERHOLDER_H)
#define _BUFFERHOLDER_H


#include "Tools.h"
#include <sys/time.h>

class WagoDevice;
class ModbusAbstract;

#define FIRMWARE_DATE_TIME_BUFFER_SIZE  16
#define FIRMWARE_DATE_TIME_BUFFER_SIZE_8WORDS  8

// Address in OW memory space used for 881 outputs cache
#define PHYS_CACHE_ADDR   384
#define PHYS_CACHE_MAX    (512-PHYS_CACHE_ADDR)

/*!Structure for storing configuration info read from Wago device thru modbus.
 *
 */
typedef struct _configInfo{
        //!WAGO series number (typically 750)
	unsigned short seriesCode; 

        //!WAGO order number (typically 842 or 881 or 862)
	unsigned short couplerControllerCode; 

        //!Firmware version (256 * major + minor) e.g.: V2.5 = 0x0205
	unsigned short firmwareVer; 

	//!Details of manufacture date of the firmware version,
	//max. 31 characters. If not available the value 0xFF appears.
	unsigned short firmwareDate[FIRMWARE_DATE_TIME_BUFFER_SIZE];

        //!Details of the manufacture time of the
	//firmware version, max. 31 characters. If
	//not available the value 0xFF appears.
	unsigned short firmwareTime[FIRMWARE_DATE_TIME_BUFFER_SIZE];
        
        //!Number of channels used by modules
	unsigned short anaOutBits;
	unsigned short anaInBits;
	unsigned short digiOutBits;
	unsigned short digiInBits;
	unsigned short physCache[PHYS_CACHE_MAX];
	unsigned short physCacheUsed;

        //! list of module codes.
	vector<short> modules;

	_configInfo& operator= (const _configInfo &x)
	{
		if( this != &x)
		{
			this->couplerControllerCode = x.couplerControllerCode;
			this->seriesCode = x.seriesCode;
			this->firmwareVer = x.firmwareVer;
			memcpy(this->firmwareDate, x.firmwareDate, FIRMWARE_DATE_TIME_BUFFER_SIZE*sizeof(unsigned short));
			memcpy(this->firmwareTime, x.firmwareTime, FIRMWARE_DATE_TIME_BUFFER_SIZE*sizeof(unsigned short));
			this->modules = x.modules;
		}
		return *this;

	}

}configInfo;

/*!Class responsible for managing buffers(and access to them) for input and output of process image.
 *
 */
class BufferHolder
{
public:
	BufferHolder();
	virtual ~BufferHolder();

	int TryReadLock();
	void ReadLock();
	void ReadUnlock();

	void WriteLock();
	void WriteUnlock();

	void UpdateProcessImage();
	void UpdateProcessImage(vector<unsigned short>& digitalInputs, vector<unsigned short>& inputRegisters,
			vector<unsigned short>& coils, vector<unsigned short>& holdingRegisters );
	void UploadOutputs();
	void CancelWrite();
	unsigned short* GetInputRegistersBuffAddr(){ return inputRegisters; };
	unsigned short* GetHoldingRegistersBuffAddr(){ return holdingRegisters; };
	unsigned short* GetCoilsBuffAddr(){ return coils; };
	unsigned short* GetDigitalInputsBuffAddr(){ return digitalInputs; };
	unsigned int GetNumOfDigitalInputs(){ return numOfDigitalInputs; };
	unsigned int GetNumOfCoils(){ return numOfCoils; };
	unsigned int GetNumOfHoldingRegisters(){ return numOfHoldingRegisters; };
	unsigned int GetNumOfInputRegisters(){ return numOfInputRegisters; };
	unsigned int GetHoldingRegistersForWrite(unsigned short& addr, vector<unsigned short>& values, bool startFromBegin = false);
	unsigned int GetCoilsForWrite(unsigned short& addr, vector<unsigned short>& values, bool startFromBegin = false);

	void SetImageValidity(bool validity);
	void ValidateImage();


	void GetConfigInformation(configInfo &cfg);

	ModbusAbstract* getModbusInterface() const {
		return modbusInterface;
	}

	void StartThread();
	void SetWagoDevice(WagoDevice* wDev){wd = wDev;};
	void PollingThreadRoutine();

protected:
	unsigned int numOfDigitalInputs;	//!Size of digital inputs buffer
	unsigned int numOfCoils;	//!Size of coils inputs buffer
	unsigned int numOfHoldingRegisters;	//!Size of holding registers inputs buffer
	unsigned int numOfInputRegisters;	//!Size of input registers inputs buffer
	unsigned short* inputRegisters;	//!Pointer to digital inputs input buffer
	unsigned short* holdingRegisters;	//!Pointer to holding registers input buffer
	unsigned short* coils;	//!Pointer to coils input buffer
	unsigned short* digitalInputs;	//!Pointer to digital inputs input buffer

	unsigned short* coilsWriteBuff;	//!Pointer to coils output buffer
	unsigned short* holdingRegistersWriteBuff;	//!Pointer to holding registers output buffer

	bool* coilsWriteFlags; //!Flags indicating that corresponding coil was written by user and needs to be uploaded to wago device.
	bool* holdingRegistersWriteFlags; //!Flags indicating that corresponding holding registers was written by user and needs to be uploaded to Wago device.
private:
	omni_mutex readMutex;	//!Mutex protecting access to input buffers.
	omni_mutex writeMutex;	//!Mutex protecting access to output buffers.
	omni_thread* pollingThr; //!Pointer to thread object responsible for input buffers update.

	ModbusAbstract *modbusInterface; //!Pointer to object representing Modbus communication layer.

	configInfo devConfiguration;	//!Buffer for configuration information read from Wago device.

	void VerifyState();
	void RetrieveConfig(configInfo &cfg);
	void DoDeviceCheck();

	WagoDevice* wd;//! Pointer to corresponding wago device object
	bool communicationError;//!Flag for input buffer update routine indicating that there was error in modbus communication.
	volatile bool closeThread;//!Flag for input buffer update routine indicating that routine shall exit.
	int update_period;//!Input buffers update period.

	bool imageValid;//!Flag indicating validity of input buffers(TRUE - image is valid).

friend class Configurator;

	LOGGING_ADDONS;
};

#endif  //_BUFFERHOLDER_H
