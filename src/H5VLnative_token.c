/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF5/releases.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Purpose:     Object token callbacks for the native VOL connector
 */

/***********/
/* Headers */
/***********/
#include "H5private.h"          /* Generic Functions        */
#include "H5Eprivate.h"         /* Error handling           */
#include "H5MMprivate.h"        /* Memory handling          */
#include "H5VLnative_private.h" /* Native VOL connector     */

/****************/
/* Local Macros */
/****************/


/******************/
/* Local Typedefs */
/******************/


/********************/
/* Local Prototypes */
/********************/


/*********************/
/* Package Variables */
/*********************/


/*****************************/
/* Library Private Variables */
/*****************************/


/*******************/
/* Local Variables */
/*******************/



/*---------------------------------------------------------------------------
 * Function:    H5VL__native_token_cmp
 *
 * Purpose:     Compare two of the connector's object tokens, setting
 *              *cmp_value, following the same rules as strcmp().
 *
 * Return:      Success:    0
 *              Failure:    (can't fail)
 *
 *---------------------------------------------------------------------------
 */
herr_t
H5VL__native_token_cmp(hid_t loc_id, const h5token_t *token1, const h5token_t *token2, int *cmp_value)
{
    herr_t ret_value = SUCCEED;

    FUNC_ENTER_PACKAGE_NOERR

    /* Check parameters */
    HDassert(token1);
    HDassert(token2);

    /* Silence compiler about unused parameter */
    (void)loc_id;

    *cmp_value = HDmemcmp(token1, token2, sizeof(h5token_t));

    FUNC_LEAVE_NOAPI(ret_value)
} /* end H5VL__native_token_cmp() */


/*---------------------------------------------------------------------------
 * Function:    H5VL__native_token_to_str
 *
 * Purpose:     Serialize an object token into a string
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *---------------------------------------------------------------------------
 */
herr_t
H5VL__native_token_to_str(hid_t loc_id, const h5token_t *token, char **token_str)
{
    haddr_t addr;
    int addr_ndigits;
    herr_t ret_value = SUCCEED;

    FUNC_ENTER_PACKAGE

    /* Check parameters */
    HDassert(token);

    if(H5VL_native_token_to_addr(loc_id, *token, &addr) < 0)
        HGOTO_ERROR(H5E_FILE, H5E_CANTDECODE, FAIL, "can't convert object token to address")

    if(addr == 0)
        addr_ndigits = 1;
    else
        addr_ndigits = floor(log10(addr)) + 1;

    if(NULL == (*token_str = H5MM_malloc(addr_ndigits + 1)))
        HGOTO_ERROR(H5E_RESOURCE, H5E_CANTALLOC, FAIL, "can't allocate buffer for token string")

    snprintf(*token_str, addr_ndigits + 1, H5_PRINTF_HADDR_FMT, addr);

done:
    FUNC_LEAVE_NOAPI(ret_value)
} /* end H5VL__native_token_to_str() */


/*---------------------------------------------------------------------------
 * Function:    H5VL__native_str_to_token
 *
 * Purpose:     Deserialize a string into an object token
 *
 * Return:      Success:    0
 *              Failure:    -1
 *
 *---------------------------------------------------------------------------
 */
herr_t
H5VL__native_str_to_token(hid_t loc_id, const char *token_str, h5token_t *token)
{
    haddr_t addr;
    herr_t ret_value = SUCCEED;

    FUNC_ENTER_PACKAGE

    /* Check parameters */
    HDassert(token_str);

    sscanf(token_str, H5_PRINTF_HADDR_FMT, &addr);

    if(H5VL_native_addr_to_token(loc_id, addr, token) < 0)
        HGOTO_ERROR(H5E_FILE, H5E_CANTDECODE, FAIL, "can't convert address to object token")

done:
    FUNC_LEAVE_NOAPI(ret_value)
} /* end H5VL__native_str_to_token() */


/*-------------------------------------------------------------------------
 * Function:    H5VL__native_free_token_str
 *
 * Purpose:     Frees an object token string
 *
 * Return:      Success:    0
 *              Failure:    (can't fail)
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5VL__native_free_token_str(hid_t loc_id, char *token_str)
{
    herr_t ret_value = SUCCEED;

    FUNC_ENTER_PACKAGE_NOERR

    /* Silence compiler about unused parameter */
    (void)loc_id;

    H5MM_xfree(token_str);

    FUNC_LEAVE_NOAPI(ret_value)
} /* end H5VL__native_free_token_str() */
