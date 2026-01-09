#include "drv_dw1000_assert.h"
#include "stdint.h"

#ifdef  USE_DW1000_FULL_ASSERT
__weak void DW1000_AssertFailed(uint8_t* file, uint32_t line){

}
#endif /* USE_DW1000_FULL_ASSERT */