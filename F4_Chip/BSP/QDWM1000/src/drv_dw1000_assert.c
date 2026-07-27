#include "drv_dw1000_assert.h"
#include "stdint.h"

#if  (USE_DW1000_FULL_ASSERT == 1U)
#define DW1000_ASSERT_PARAM(expr) ((expr) ? (void)0U : DW1000_AssertFailed((uint8_t *)__FILE__, __LINE__))
void DW1000_AssertFailed(uint8_t* file, uint32_t line){

}
#else
#define DW1000_ASSERT_PARAM(expr) ((void)0U)
#endif /* USE_DW1000_FULL_ASSERT */