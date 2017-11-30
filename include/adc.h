#ifndef _ADC_H_
#define _ADC_H_

#include <stdint.h>
#include <gpio.h>

#ifndef ADC_TIMEOUT_TICKS
#define ADC_TIMEOUT_TICKS   500000
#endif /* ADC_TIMEOUT_TICKS */

/* If we are in an interrupt and have checked once already then we need to
 * exit with a timeout to allow other interrupts to be serviced as quickly
 * as possible. */
#define ADC_WITH_TIMEOUT(do_with_timeout)\
  for (unsigned int i = 0; do_with_timeout; ++i) {\
    if ((adc_within_interrupt == true && i > 1) || i > ADC_TIMEOUT_TICKS) {\
      return ADC_TIMEOUT;\
    }\
  }
#define ADC_VOLTS(X)        ((X) / 1000)
#define ADC_MILLIVOLTS(X)   (((X) % 1000) / 100)

typedef enum {
  ADC_STATUS_OK,
  ADC_STATUS_TIMEOUT,
  ADC_STATUS_INVALID_CONVERT_PIN,
  ADC_STATUS_CONVERSION_IN_PROGRESS,
  ADC_STATUS_NEED_CONVERSION_CALLBACK,
} adc_status_code;

typedef uint32_t adc_status_data_t;

typedef struct {
  adc_status_code code;
  adc_status_data_t data;
} adc_status_t;

#define ADC_ERROR(status)    (status.code != ADC_STATUS_OK)
#define ADC_FATAL(status, func)    status = func; \
  if (ADC_ERROR(status)) {\
  assert_failed(__FILE__, __LINE__); \
}
extern adc_status_t ADC_OK;
extern adc_status_t ADC_TIMEOUT;
extern adc_status_t ADC_INVALID_CONVERT_PIN;
extern adc_status_t ADC_CONVERSION_IN_PROGRESS;
extern adc_status_t ADC_NEED_CONVERSION_CALLBACK;

typedef adc_status_t adc_convertion_result;

extern adc_status_t (*adc_conversion_complete)(adc_convertion_result result);

adc_status_t adc_calibration();
adc_status_t adc_enable();
adc_status_t adc_read();
adc_status_t adc_select_conversion_pin(gpio_pin_t pin_to_convert);
adc_status_t adc_watch_enable(gpio_pin_t pin_to_convert,
    uint16_t vrefint_low, uint16_t vrefint_high);
adc_status_t adc_up(gpio_pin_t gpio_pin);
adc_status_t adc_down();
adc_status_t adc_convert(gpio_pin_t pin_to_convert);
adc_status_t adc_convert_async(gpio_pin_t pin_to_convert,
    adc_status_t (*adc_conversion_complete)(adc_convertion_result result));

void adc_handler();

#endif /* _ADC_H_ */
