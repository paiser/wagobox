static const char *RcsId = "$Id: BufferHolder.cpp,v 1.10 2018/06/15 12:51:59 perez Exp $";
//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Wago Modbus Tango Server
//  @ File Name : BufferHolder.cpp
//  @ Date : 2013-02-03
//  @ Author : Pawel Kowaliszyn
//
//


#include "BufferHolder.h"
#include "ModbusAbstract.h"
#include "WagoDevice.h"

#include <iostream>

#define WAGO_OUTPUTIMAGE_OFFSET 0x0200

/*!Trampoline to run PollingThreadRoutine routine
 *
 */
extern "C" void* polling_proc(void *arg)
{
	static_cast<BufferHolder*>(arg)->PollingThreadRoutine();
}

/* The default constructor
 *
 */
BufferHolder::BufferHolder():
numOfDigitalInputs(0),
numOfCoils(0),
numOfHoldingRegisters(0),
numOfInputRegisters(0),
inputRegisters(NULL),
holdingRegisters(NULL),
coils(NULL),
digitalInputs(NULL),
holdingRegistersWriteBuff(NULL),
coilsWriteBuff(NULL),
coilsWriteFlags(NULL),
holdingRegistersWriteFlags(NULL),
modbusInterface(NULL),
imageValid(false),
pollingThr(NULL)
{
	this->update_period= 1000;
}

/*!The destructor. Releases memory alocated for buffers and clears the context.
 *
 */
BufferHolder::~BufferHolder()
{
	DEBUG_STREAM << "BufferHolder::~BufferHolder() called" <<endl;

	if(pollingThr)
	{
		closeThread = true;
		pollingThr->join(NULL);
	}

	DEBUG_STREAM << "BufferHolder::~BufferHolder() pollingThr->join(NULL); returned." <<endl;

	if(modbusInterface)
	{
		delete modbusInterface;
	}

	if(inputRegisters)
		free(inputRegisters);

	if(holdingRegisters)
		free(holdingRegisters);

	if(coils)
		free(coils);

	if(digitalInputs)
		free(digitalInputs);

	if(holdingRegistersWriteBuff)
		free(holdingRegistersWriteBuff);

	if(coilsWriteBuff)
		free(coilsWriteBuff);

	if(coilsWriteFlags)
		free(coilsWriteFlags);

	if(holdingRegistersWriteFlags)
		free(holdingRegistersWriteFlags);

}

/*!This method attempts to lock the input buffers access mutex and returns immediately if failed (non blocking lock attempt)
 *
 *	\return  operation status true if attempt succeeded , false if failed.
 */
int BufferHolder::TryReadLock()
{
	return readMutex.trylock();
}

/*!Method for locking access to input buffers (blocking)
 *
 */
void BufferHolder::ReadLock()
{
	readMutex.lock();
}

/*!Method for releasing access to input buffers (blocking)
 *
 */
void BufferHolder::ReadUnlock()
{
	readMutex.unlock();
}


/*!Method for locking access to output buffers (blocking)
 *
 */
void BufferHolder::WriteLock()
{
	writeMutex.lock();
}

/*!Method for locking access to output buffers (blocking)
 *
 */
void BufferHolder::WriteUnlock()
{
	writeMutex.unlock();
}

/* Updates input buffers with values supplied by modbus layer.
 *
 *\param digitalInputs	values of digital inputs
 *\param inputRegisters	values of digital inputRegisters
 *\param coils	values of digital coils
 *\param holdingRegisters	values of digital holdingRegisters
 */
void BufferHolder::UpdateProcessImage(vector<unsigned short>& digitalInputs, vector<unsigned short>& inputRegisters,vector<unsigned short>& coils, vector<unsigned short>& holdingRegisters )
{
	if( digitalInputs.size() != this->numOfDigitalInputs ||
		holdingRegisters.size() != this->numOfHoldingRegisters ||
		coils.size() != this->numOfCoils ||
		inputRegisters.size() != this->numOfInputRegisters)
	{
		ERROR_STREAM << "UpdateProcessImage - invalid received process image size " << endl;
		EXCEPTION_RETURN("BufferHolder::UpdateProcessImage","Invalid received process image size", "Modbus::error_read");
	}

	ReadLock();
	copy(digitalInputs.begin(), digitalInputs.end(), this->digitalInputs);
	copy(inputRegisters.begin(), inputRegisters.end(), this->inputRegisters);
	copy(coils.begin(), coils.end(), this->coils);
	copy(holdingRegisters.begin(), holdingRegisters.end(), this->holdingRegisters);
	imageValid = true;

	ReadUnlock();
}

/*!Method returns the block of neighboring holding registers which have been written and pending for sending to modbus.
 * Method stops search after on end of block, current position in buffer is saved. During next call search will be started
 * from this position (if startFromBegin is not set) . This method should be called in loop until returns 0 which means
 * that there is no more registers for update.
 *
 * \param addr	reference to a placeholder which will indicate addres of first register in block.
 * \param values reference to placeholder for values of holding registers.
 * \param startFromBegin	flag indicating that search should be started from the beginning (default false).
 *
 * \return number of registers in block.
 *
 */
unsigned int BufferHolder::GetHoldingRegistersForWrite(unsigned short& addr, vector<unsigned short>& values, bool startFromBegin)
{
	static unsigned int ctr = 0;
	unsigned int i;

	unsigned int numberOfPoints = 0;
	if(startFromBegin)
		ctr = 0;


        INFO_STREAM << "BufferHolder::GetHoldingRegistersForWrite:" << endl;
        INFO_STREAM << "    ctr:" << ctr << endl;
	for(i=0; i<numOfHoldingRegisters ; i++)
                INFO_STREAM << "    holdingRegistersWriteFlags[" << i << "]:" << holdingRegistersWriteFlags[i] << endl;

	addr = ctr;
	for(; ctr < numOfHoldingRegisters ; ctr++)
	{
		if(!holdingRegistersWriteFlags[ctr])
		{
			if(numberOfPoints)
				return numberOfPoints;

			continue;
		}

		if(numberOfPoints == 0)
		        addr = ctr;

		numberOfPoints++;
		values.push_back(this->holdingRegistersWriteBuff[ctr]);
		this->holdingRegistersWriteFlags[ctr] = false;
	}

	ctr = 0;
	return numberOfPoints;
}

/*!Method returns the block of neighboring coils which have been written and pending for sending to modbus.
 * Method stops search after on end of block, current position in buffer is saved. During next call search will be started
 * from this position (if startFromBegin is not set) . This method should be called in loop until returns 0 which means
 * that there is no more registers for update.
 *
 * \param addr	reference to a placeholder which will indicate addres of first register in block.
 * \param values reference to placeholder for values of coilss.
 * \param start	flag indicating that search should be started from the beginning (default false).
 *
 * \return number of coils in block.
 *
 */
unsigned int BufferHolder::GetCoilsForWrite(unsigned short& addr, vector<unsigned short>& values, bool startFromBegin)
{
	static unsigned int ctr = 0;
	unsigned int i;

	unsigned int numberOfPoints = 0;
	if(startFromBegin)
		ctr = 0;

        INFO_STREAM << "BufferHolder::GetCoilsForWrite:" << endl;
        INFO_STREAM << "    ctr:" << ctr << endl;
	for(i=0; i<numOfCoils ; i++)
                INFO_STREAM << "    coilsWriteFlags[" << i << "]:" << coilsWriteFlags[i] << endl;

	addr = ctr;
	for(; ctr < numOfCoils ; ctr++)
	{
		if(!coilsWriteFlags[ctr])
		{
			if(numberOfPoints)
				return numberOfPoints;

			continue;
		}

		if(numberOfPoints == 0)
			addr = ctr;

		numberOfPoints++;
		values.push_back(coilsWriteBuff[ctr]);
		coilsWriteFlags[ctr] = false;
	}

	ctr = 0;
	return numberOfPoints;
}

/*!Cancel all changes in write buffer. Flag buffers for written values will be set to zero
 *
 */
void BufferHolder::CancelWrite()
{

	if(coilsWriteFlags)
		memset(coilsWriteFlags, 0x0, numOfCoils*sizeof(bool));

	if(holdingRegistersWriteFlags)
		memset(holdingRegistersWriteFlags, 0x0, numOfHoldingRegisters*sizeof(bool));
}

/*!Set validity flag for input buffers.
 *
 * \param validity	if image is valid true, if image is invalid false.
 */
void BufferHolder::SetImageValidity(bool validity)
{
	ReadLock();
	imageValid = validity;
	ReadUnlock();

}

/*!Check validity of process image (input buffers). If image is invalid
 * last caught exception will be thrown.
 *
 */
void BufferHolder::ValidateImage()
{
	if(!imageValid)
		modbusInterface->RaiseLastModbusException();
}

/*!Returns the configuration of Wago device seen by modbus.
 *
 *\param cfg	a placeholder for configuration structure.
 */
void BufferHolder::GetConfigInformation(configInfo &cfg)
{
	cfg = devConfiguration;
}

/*!Read the module configuration(through modbus) of Wago device and store it in buffer.
 *
 *\param cfg	reference to buffer.
 *
 */
void BufferHolder::RetrieveConfig(configInfo &cfg)
{
	vector<unsigned short> values;
        unsigned int nwords;

	cfg.couplerControllerCode = 0;
	memset(cfg.firmwareDate,0,FIRMWARE_DATE_TIME_BUFFER_SIZE);
	memset(cfg.firmwareTime,0,FIRMWARE_DATE_TIME_BUFFER_SIZE);
	cfg.firmwareVer = 0;
	cfg.seriesCode = 0;
	cfg.modules.clear();

	try{
		modbusInterface->ReadInputRegisters(0x2011, 1, values); //Read controller series code
		cfg.seriesCode = values[0];
		values.clear();
		modbusInterface->ReadInputRegisters(0x2012, 1, values);//Read controller series code
		cfg.couplerControllerCode = values[0];
		INFO_STREAM << "BufferHolder::RetrieveConfig controller: " << cfg.couplerControllerCode << endl;

		// Get information of modules seen physically by the controller
		if (cfg.couplerControllerCode == 881 || cfg.couplerControllerCode == 862) { 
			values.clear();
			modbusInterface->ReadInputRegisters(0x1022, 4, values);

			INFO_STREAM << "BufferHolder::RetrieveConfig modules using:" << endl;
			INFO_STREAM << "    Ana  Out: " << values[0] << "bits" << endl;
			INFO_STREAM << "    Ana  In:  " << values[1] << "bits" << endl;
			INFO_STREAM << "    Digi Out: " << values[2] << "bits" << endl;
			INFO_STREAM << "    Digi In:  " << values[3] << "bits" << endl;

                        cfg.anaOutBits    = values[0];
                        cfg.anaInBits     = values[1];
                        cfg.digiOutBits   = values[2];
                        cfg.digiInBits    = values[3];
                        cfg.physCacheUsed = (cfg.anaOutBits/16) + int(cfg.digiOutBits/16) + 1;
			INFO_STREAM << "BufferHolder::RetrieveConfig cache size for outputs:  " << \
                                cfg.physCacheUsed << "words" << endl;

                        memset(cfg.physCache, 0, sizeof(cfg.physCache));
		} 

		// 16 words by default but for two Ethernet coupler or CPU
		nwords = FIRMWARE_DATE_TIME_BUFFER_SIZE; 
		// but for two Ethernet coupler or CPU
		if (cfg.couplerControllerCode == 881) { 
			nwords = FIRMWARE_DATE_TIME_BUFFER_SIZE_8WORDS;
		} 
		if (cfg.couplerControllerCode == 352 || cfg.couplerControllerCode == 862) { 
			nwords = FIRMWARE_DATE_TIME_BUFFER_SIZE_8WORDS;
		} 
 

		values.clear();
		modbusInterface->ReadInputRegisters(0x2010, 1, values);//Read firmware version
		cfg.firmwareVer = values[0];
		values.clear();
		modbusInterface->ReadInputRegisters(0x2022, nwords, values);//Read firmware date
		for(unsigned int i = 0; (i < values.size()) && (i < nwords); i++)
		{
			cfg.firmwareDate[i] = values[i];
		}

		values.clear();
		modbusInterface->ReadInputRegisters(0x2021 /*- 0x0200*/, nwords, values); //Read firmware time
		DEBUG_STREAM << "Read firmware time" <<endl;
		for(unsigned int i = 0; (i < values.size()) && (i < nwords); i++)
		{
			cfg.firmwareTime[i] = values[i];
		}

		//TODO firmware version check
		DEBUG_STREAM << "BufferHolder::RetrieveConfig reading module list " << endl;
		values.clear();
		modbusInterface->ReadInputRegisters(0x2030 /*- 0x0200*/, 65, values);//Read module list seen by controller
		for(unsigned int i = 1; i < values.size() && values[i] ; i++) // first module is in fact controller
		{
			cfg.modules.push_back(values[i]);
		}

		if(cfg.modules.size() == 65)
		{
			values.clear();
			modbusInterface->ReadInputRegisters(0x2031 /* - 0x0200*/, 64, values); //Read module list seen by controller
			for(unsigned int i = 0; i < values.size() && values[i]; i++)
			{
				cfg.modules.push_back(values[i]);
			}
		}
		if(cfg.modules.size() == 129)
		{
			values.clear();
			modbusInterface->ReadInputRegisters(0x2032 /*- 0x0200*/, 64, values); //Read module list seen by controller
			for(unsigned int i = 0; i < values.size() && values[i]; i++)
			{
				cfg.modules.push_back(values[i]);
			}
		}
		if(cfg.modules.size() == 193)
		{
			values.clear();
			modbusInterface->ReadInputRegisters(0x2033 /*- 0x0200*/, 63, values); //Read module list seen by controller
			for(unsigned int i = 0; i < values.size() && values[i]; i++)
			{
				cfg.modules.push_back(values[i]);
			}
		}
	}
	catch(...){}

}

/*!Routine for updating process image buffers. Routine has infinite loop and should be run in separate thread.
 *
 */
void BufferHolder::PollingThreadRoutine()
{
	struct timespec dodo;
	struct timeval  tv;
	struct timezone tz;
	int    dodo_msec;
	struct timeval  cache_update_t;


	DoDeviceCheck();

	INFO_STREAM <<
			"BufferHolder::PollingThreadRoutine() entering polling loop!"
			<< endl;
	while (closeThread != true) {
		if (communicationError) {
			DoDeviceCheck();
			continue;
		} else {
			try {
				UpdateProcessImage();
			} catch (...) {
			} //nothing to do here exception handling is done in UpdateProcessImage, exception needs to be catched to not stop thread;
		}
		//
		// Timestamp the cache update
		//
		gettimeofday(&cache_update_t, &tz);

		//
		// Sleep for a while if previous cache update took place before
		// predefined period time has elapsed
		//
		do {
			gettimeofday(&tv, &tz);
			dodo_msec = (tv.tv_sec - cache_update_t.tv_sec) * 1000
					+ (tv.tv_usec - cache_update_t.tv_usec) / 1000;

			if (dodo_msec > this->update_period)
				dodo_msec = 0;
			else
				dodo_msec = this->update_period - dodo_msec;

			if (dodo_msec > 0) {
				dodo.tv_sec = (dodo_msec / 1000);
				dodo.tv_nsec = (dodo_msec % 1000) * 1000000;

				nanosleep(&dodo, NULL);
			}
		} while (dodo_msec > 0);
	}


	DEBUG_STREAM << "Periodic update thread PollingThreadRoutine returns"<<endl;
}

/*! Verify if modbus devices state is valid. If status is invalid flag for update thread will be set.
 *
 */
void BufferHolder::VerifyState()
{
	communicationError = modbusInterface->ModbusStateCheck();

	if(communicationError)
		INFO_STREAM << "BufferHolder::VerifyState() Invalid Modbus state buffer update stopped." <<endl;
	else
		ERROR_STREAM << "BufferHolder::VerifyState() Modbus state valid buffer update started." <<endl;

}


/*!Method reads process image values from modbus layer and writes them into input buffers.
 *
 */
void BufferHolder::UpdateProcessImage()
{
	vector<unsigned short> digitalInputs;
	vector<unsigned short> inputRegisters;
	vector<unsigned short> coils;
	vector<unsigned short> holdingRegisters;

	modbusInterface->LockAccess();
	try
	{
		if(GetNumOfInputRegisters())
			modbusInterface->ReadInputRegisters(0,GetNumOfInputRegisters(), inputRegisters);

		if(GetNumOfDigitalInputs())
			modbusInterface->ReadDigitalInputs(0,GetNumOfDigitalInputs(), digitalInputs);

		if(GetNumOfHoldingRegisters())
			modbusInterface->ReadHoldingRegisters(WAGO_OUTPUTIMAGE_OFFSET,GetNumOfHoldingRegisters(), holdingRegisters);

		if(GetNumOfCoils())
			modbusInterface->ReadMultipleCoilsStatus(WAGO_OUTPUTIMAGE_OFFSET,GetNumOfCoils(), coils);
	}
	catch(...)
	{
		ERROR_STREAM << "UpdateProcessImage caught exception while read attempt" << endl;
		VerifyState();
		if(!communicationError)
			this->modbusInterface->InconsistentImageFault("Communication error while reading process image");

		SetImageValidity(false);
		modbusInterface->UnlockAccess();
		throw;
	}

	modbusInterface->UnlockAccess();

	try
	{
		UpdateProcessImage(digitalInputs, inputRegisters, coils, holdingRegisters);
		SetImageValidity(true);
		modbusInterface->ImageConsistent();
	}
	catch(...)
	{
		ERROR_STREAM << "UpdateProcessImage caught exception while read attempt" << endl;

		SetImageValidity(false);
		modbusInterface->InconsistentImageFault("Invalid size of downloaded process image data");
		throw;
	}
}

/*! Interface function for uploading output buffer values to modbus.
 *
 */
void BufferHolder::UploadOutputs()
{
	unsigned int num;
	vector<unsigned short> buff;
	unsigned short addr;
        unsigned int i;

	modbusInterface->LockAccess();
	try{
		while(num = GetHoldingRegistersForWrite(addr,buff))
		{
                        INFO_STREAM << "UploadOutputs:PresetMultipleRegisters:" << endl;
                        INFO_STREAM << "    modbus address:"<< addr << endl;
                        for(i=0;i<buff.size();i++)
                             INFO_STREAM << "    registers[" << i << "]:" << buff[i] << endl;

			modbusInterface->PresetMultipleRegisters(addr, buff);
			buff.clear();
		}

		while(num = GetCoilsForWrite(addr,buff))
		{
                        INFO_STREAM << "UploadOutputs:PresetMultipleCoils:" << endl;
                        INFO_STREAM << "    modbus address:"<< addr << endl;
                        for(i=0;i<buff.size();i++)
                             INFO_STREAM << "    coils[" << i << "]:" << buff[i] << endl;
			modbusInterface->ForceMultipleCoils(addr, buff);
			buff.clear();
		}

	}catch(...)
	{
		ERROR_STREAM << "UploadOutputs caught exception while write attempt" << endl;
		VerifyState();
		SetImageValidity(false);
		modbusInterface->UnlockAccess();
		throw ;
	}
	modbusInterface->UnlockAccess();
}

/*! Method for starting up of thread for cyclic update of input bffers.
 *
 */
void BufferHolder::StartThread()
{
	closeThread = false;
	INFO_STREAM << "Starting periodic buffer update thread." <<endl;
	pollingThr = new omni_thread(polling_proc, (void*)this);
	pollingThr->start();
}

/*! Check if configuration of modules read from Wago device matches configuration of modules
 *  supplied by user. If not set the invalid flag for buffers and signalize issue to modbus layer.
 */
void BufferHolder::DoDeviceCheck()
{
	configInfo cfg;

	ReadLock();
	modbusInterface->LockAccess();
	VerifyState();
	if(!communicationError)
	{
		RetrieveConfig(cfg);

		devConfiguration = cfg;

		if(!wd->ValidateConfiguration(cfg))
		{
			modbusInterface->InconsistentImageFault("Module configuration differs from configuration of Wago box");
			communicationError = true;
		}
		else
		{
			modbusInterface->ImageConsistent();
			modbusInterface->UnlockAccess();
			ReadUnlock();
			return;
		}
	}
	modbusInterface->UnlockAccess();
	ReadUnlock();
	sleep(5);

}
