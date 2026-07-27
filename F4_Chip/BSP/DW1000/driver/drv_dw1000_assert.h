/**
 * @file drv_dw1000_assert.h
 * @author quote6 (HeXiHeNian@qq.com)
 * @brief 
 * @version 0.1
 * @date 2026-07-24
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once

/* 是否使用DW1000断言功能 */
#define USE_DW1000_FULL_ASSERT        0U

#if  (USE_DW1000_FULL_ASSERT == 1U)
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr If expr is false, it calls assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed.
  *         If expr is true, it returns no value.
  * @retval None
  */
  #define DW1000_ASSERT_PARAM(expr) ((expr) ? (void)0U : DW1000_AssertFailed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void DW1000_AssertFailed(uint8_t* file, uint32_t line);
#else
  #define DW1000_ASSERT_PARAM(expr) ((void)0U)
#endif /* USE_DW1000_FULL_ASSERT */
