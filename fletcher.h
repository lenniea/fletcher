#ifndef __FLETCHER_H__
#define __FLETCHER_H__

#ifdef __cplusplus
extern "C"
#endif

#include <stdlib.h>		/* for size_t */
#include <stdint.h>		/* for uint8_t, uint16_t & uint32_t */

extern uint16_t fletcher16(const uint8_t* data, size_t len);
extern uint32_t fletcher32(const uint16_t* data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* __FLETCHER_H__ */
