/**
 * Application Header File
 *
 * @req SWS_Rte_01003
 */

/** === HEADER ====================================================================================
 */

/** --- C++ guard ---------------------------------------------------------------------------------
 * @req SWS_Rte_03709
 */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** --- Normal include guard ----------------------------------------------------------------------
 */
#ifndef RTE_IOHWAB_H_
#define RTE_IOHWAB_H_

/** --- Duplicate application include guard -------------------------------------------------------
 * @req SWS_Rte_01006
 */
#ifdef RTE_APPLICATION_HEADER_FILE
#error Multiple application header files included.
#endif
#define RTE_APPLICATION_HEADER_FILE

/** --- Single runnable API -----------------------------------------------------------------------
 * @req SWS_Rte_02751
 */
#if defined(RTE_RUNNABLEAPI_ImuSensorRead) || \
defined(RTE_RUNNABLEAPI_UltraSensorRead)
#define RTE_RUNNABLEAPI
#endif

/** --- Includes ----------------------------------------------------------------------------------
 * @req SWS_Rte_02751
 * @req SWS_Rte_07131
 */
#include <Rte_DataHandleType.h>
#include <Rte_IoHwAb_Type.h>

/** --- PIM DATA TYPES ------------------------------------------------------------------------------ */

/** === BODY ======================================================================================
 */

/** @req SWS_Rte_03731
 *  @req SWS_Rte_07137
 *  @req SWS_Rte_07138
 *  !req SWS_Rte_06523
 *  @req SWS_Rte_03730
 *  @req SWS_Rte_07677
 *  @req SWS_Rte_02620
 *  @req SWS_Rte_02621
 *  @req SWS_Rte_01055
 *  @req SWS_Rte_03726 */

/** @req SWS_Rte_01343
 *  @req SWS_Rte_01342
 *  !req SWS_Rte_06524
 *  @req SWS_Rte_01053
 */

/** @req SWS_Rte_07132
 *  @req SWS_Rte_03714 */
typedef struct {
    uint8 _dummy;
} Rte_CDS_IoHwAb;

/** --- Instance handle type ---------------------------------------------------------------------- */
typedef Rte_CDS_IoHwAb const * const Rte_Instance;

/** --- Singleton instance handle -----------------------------------------------------------------
 *  @req SWS_Rte_03793
 */
extern const Rte_Instance Rte_Inst_IoHwAb;
#define self (Rte_Inst_IoHwAb)

/** --- Calibration API --------------------------------------------------------------------------- */

/** --- Per Instance Memory API ------------------------------------------------------------------- */

/** --- Single Runnable APIs ---------------------------------------------------------------------- */
#if defined(RTE_RUNNABLEAPI)
/** --- ImuSensorRead */
#if defined(RTE_RUNNABLEAPI_ImuSensorRead)

Std_ReturnType ImuSensorRead(/*IN*/UInt16 const * portDefArg1, /*IN*/UInt16 const * Data);

#endif

/** --- UltraSensorRead */
#if defined(RTE_RUNNABLEAPI_UltraSensorRead)

Std_ReturnType UltraSensorRead(/*IN*/UInt32 portDefArg1, /*IN*/UInt16 const * Data);

#endif
#endif

/** --- All Runnable APIs ------------------------------------------------------------------------- */
#if !defined(RTE_RUNNABLEAPI)
Std_ReturnType ImuSensorRead(/*IN*/UInt16 portDefArg1, /*IN*/UInt16 const * Data);

Std_ReturnType UltraSensorRead(/*IN*/UInt32 portDefArg1, /*IN*/UInt16 const * Data);
#endif

/** === FOOTER ====================================================================================
 */

#endif /* RTE_IOHWAB_H_ */

/** @req SWS_Rte_03710 */
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
