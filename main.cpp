#include "mbed.h"
#include "ble/BLE.h"

static EventQueue event_queue(4*EVENTS_EVENT_SIZE);

DigitalOut led1(LED1);

const static char DEVICE_NAME[] = "+=+=+=+=+=+=+=+=+=+=+";

void schedule_ble_processing(BLE::OnEventsToProcessCallbackContext* context) {
    event_queue.call(callback(&(context->ble), &BLE::processEvents));
}

void on_ble_init_complete(BLE::InitializationCompleteCallbackContext *context) {
    BLE& ble_interface = context->ble;
    ble_error_t initialization_error = context->error;
    if (initialization_error) {
        // handle error
        return;
    }
}

// main() runs in its own thread in the OS
int main()
{
    led1 = 1;
    BLE &ble_interface = BLE::Instance();

    /* Setup advertising */
    ble_interface.gap().accumulateAdvertisingPayload(GapAdvertisingData::GENERIC_HEART_RATE_SENSOR);
    ble_interface.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble_interface.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);

    ble_interface.gap().setAdvertisingInterval(1000);
    ble_interface.gap().startAdvertising();

    ble_interface.init(on_ble_init_complete);
    ble_interface.onEventsToProcess(schedule_ble_processing);
    event_queue.dispatch_forever();

    return 0;
}
