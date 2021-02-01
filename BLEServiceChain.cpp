#include "BLEServiceChain.h"

BLEServiceChain::BLEServiceChain(): service(NULL), next(NULL){}

BLEServiceChain::BLEServiceChain(BLEService *service):service(service), next(NULL){}

void BLEServiceChain::add(BLEService *service)
{
    if (this->service == NULL)
    {
        this->service = service;
    }
    else if (this->next == NULL)
    {
        this->next = new BLEServiceChain(service);
    }
    else
    {
        this->next->add(service);
    }
}

void BLEServiceChain::checkWriteAccess(const GattWriteCallbackParams *params){
    if(this->service == NULL) return;
    //Todo check for writable chars first
    bool res = this->service->checkWriteAccess(params);
    if (!res && this->next != NULL)
    {
        this->next->checkWriteAccess(params);
    }
}

void BLEServiceChain::checkReadAccess(const GattReadCallbackParams *params){
    if(this->service == NULL) return;
    //Todo check for readable chars first
    bool res = this->service->checkReadAccess(params);
    if (!res && this->next != NULL)
    {
        this->next->checkReadAccess(params);
    }
}

void BLEServiceChain::checkNotifyRegistrations(GattAttribute::Handle_t handle, bool enable){
    if(this->service == NULL) return;
    //Todo check for notifiable chars first
    bool res = this->service->checkNotifyRegistrations(handle, enable);
    if (!res && this->next != NULL)
    {
        this->next->checkNotifyRegistrations(handle, enable);
    } 
}

void BLEServiceChain::init(){
    if(this->service == NULL) return;
    this->service->init();
    if (this->next != NULL)
    {
        this->next->init();
    } 
}

void BLEServiceChain::pastBleInit(){
    if(this->service == NULL) return;
    this->service->pastBleInit();
    if (this->next != NULL)
    {
        this->next->pastBleInit();
    } 
}

void BLEServiceChain::initCustomGattServices(){
    // printf("[bleServiceChain] initCustomGattServices\r\n");
    if(this->service == NULL) {
        printf("[bleServiceChain] ERROR no entry in chain");
        return;
    }
    this->service->initCharacteristics();
    this->service->initService();
    BLE &ble = BLE::Instance();
    ble.gattServer().addService(*this->service->getGattService());

    if (this->next != NULL)
    {
        this->next->initCustomGattServices();
    }
}