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
 * Purpose:     The native VOL connector where access is to a single HDF5 file
 *              using HDF5 VFDs.
 */

#include "H5private.h"          /* Generic Functions                        */
#include "H5Eprivate.h"         /* Error handling                           */
#include "H5Fprivate.h"         /* Files                                    */
#include "H5Iprivate.h"         /* IDs                                      */
#include "H5Pprivate.h"         /* Property lists                           */
#include "H5VLprivate.h"        /* Virtual Object Layer                     */

#include "H5VLnative_private.h" /* Native VOL connector                     */


/* The VOL connector identification number */
static hid_t H5VL_NATIVE_ID_g = H5I_INVALID_HID;

/* Prototypes */
static herr_t H5VL__native_term(void);

/* Native VOL connector class struct */
static const H5VL_class_t H5VL_native_cls_g = {
    H5VL_NATIVE_VERSION,                            /* version      */
    H5VL_NATIVE_VALUE,                              /* value        */
    H5VL_NATIVE_NAME,                               /* name         */
    0,                                              /* capability flags */
    NULL,                                           /* initialize   */
    H5VL__native_term,                              /* terminate    */
    {   /* info_cls */
        (size_t)0,                                  /* info size    */
        NULL,                                       /* info copy    */
        NULL,                                       /* info compare */
        NULL,                                       /* info free    */
        NULL,                                       /* info to str  */
        NULL                                        /* str to info  */
    },
    {   /* wrap_cls */
        NULL,                                       /* get_object   */
        NULL,                                       /* get_wrap_ctx */
        NULL,                                       /* wrap_object  */
        NULL,                                       /* unwrap_object */
        NULL                                        /* free_wrap_ctx */
    },
    {   /* attribute_cls */
        H5VL__native_attr_create,                   /* create       */
        H5VL__native_attr_open,                     /* open         */
        H5VL__native_attr_read,                     /* read         */
        H5VL__native_attr_write,                    /* write        */
        H5VL__native_attr_get,                      /* get          */
        H5VL__native_attr_specific,                 /* specific     */
        H5VL__native_attr_optional,                 /* optional     */
        H5VL__native_attr_close                     /* close        */
    },
    {   /* dataset_cls */
        H5VL__native_dataset_create,                /* create       */
        H5VL__native_dataset_open,                  /* open         */
        H5VL__native_dataset_read,                  /* read         */
        H5VL__native_dataset_write,                 /* write        */
        H5VL__native_dataset_get,                   /* get          */
        H5VL__native_dataset_specific,              /* specific     */
        H5VL__native_dataset_optional,              /* optional     */
        H5VL__native_dataset_close                  /* close        */
    },
    {   /* datatype_cls */
        H5VL__native_datatype_commit,               /* commit       */
        H5VL__native_datatype_open,                 /* open         */
        H5VL__native_datatype_get,                  /* get          */
        H5VL__native_datatype_specific,             /* specific     */
        NULL,                                       /* optional     */
        H5VL__native_datatype_close                 /* close        */
    },
    {   /* file_cls */
        H5VL__native_file_create,                   /* create       */
        H5VL__native_file_open,                     /* open         */
        H5VL__native_file_get,                      /* get          */
        H5VL__native_file_specific,                 /* specific     */
        H5VL__native_file_optional,                 /* optional     */
        H5VL__native_file_close                     /* close        */
    },
    {   /* group_cls */
        H5VL__native_group_create,                  /* create       */
        H5VL__native_group_open,                    /* open         */
        H5VL__native_group_get,                     /* get          */
        H5VL__native_group_specific,                /* specific     */
        H5VL__native_group_optional,                /* optional     */
        H5VL__native_group_close                    /* close        */
    },
    {   /* link_cls */
        H5VL__native_link_create,                   /* create       */
        H5VL__native_link_copy,                     /* copy         */
        H5VL__native_link_move,                     /* move         */
        H5VL__native_link_get,                      /* get          */
        H5VL__native_link_specific,                 /* specific     */
        NULL                                        /* optional     */
    },
    {   /* object_cls */
        H5VL__native_object_open,                   /* open         */
        H5VL__native_object_copy,                   /* copy         */
        H5VL__native_object_get,                    /* get          */
        H5VL__native_object_specific,               /* specific     */
        H5VL__native_object_optional                /* optional     */
    },
    {   /* introspect_cls */
        H5VL__native_introspect_get_conn_cls,       /* get_conn_cls */
        H5VL__native_introspect_opt_query,          /* opt_query    */
    },
    {   /* request_cls */
        NULL,                                       /* wait         */
        NULL,                                       /* notify       */
        NULL,                                       /* cancel       */
        NULL,                                       /* specific     */
        NULL,                                       /* optional     */
        NULL                                        /* free         */
    },
    {   /* blob_cls */
        H5VL__native_blob_put,                      /* put */
        H5VL__native_blob_get,                      /* get */
        H5VL__native_blob_specific,                 /* specific */
        NULL                                        /* optional */
    },
    NULL                                            /* optional     */
};


/*-------------------------------------------------------------------------
 * Function:    H5VL_native_register
 *
 * Purpose:     Register the native VOL connector and retrieve an ID for it.
 *
 * Return:      Success:    The ID for the native connector
 *              Failure:    H5I_INVALID_HID
 *
 *-------------------------------------------------------------------------
 */
hid_t
H5VL_native_register(void)
{
    hid_t ret_value = H5I_INVALID_HID;            /* Return value */

    FUNC_ENTER_NOAPI(H5I_INVALID_HID)

    /* Register the native VOL connector, if it isn't already */
    if(H5I_INVALID_HID == H5VL_NATIVE_ID_g)
        if((H5VL_NATIVE_ID_g = H5VL_register_connector(&H5VL_native_cls_g, TRUE, H5P_DEFAULT)) < 0)
            HGOTO_ERROR(H5E_VOL, H5E_CANTINSERT, H5I_INVALID_HID, "can't create ID for native VOL connector")

    /* Set return value */
    ret_value = H5VL_NATIVE_ID_g;

done:
    FUNC_LEAVE_NOAPI(ret_value)
} /* end H5VL_native_register() */


/*---------------------------------------------------------------------------
 * Function:    H5VL__native_term
 *
 * Purpose:     Shut down the native VOL
 *
 * Returns:     SUCCEED (Can't fail)
 *
 *---------------------------------------------------------------------------
 */
static herr_t
H5VL__native_term(void)
{
    FUNC_ENTER_STATIC_NOERR

    /* Reset VOL ID */
    H5VL_NATIVE_ID_g = H5I_INVALID_HID;

    FUNC_LEAVE_NOAPI(SUCCEED)
} /* end H5VL__native_term() */


/*---------------------------------------------------------------------------
 * Function:    H5VL__native_introspect_get_conn_cls
 *
 * Purpose:     Query the connector class.
 *
 * Note:        This routine is in this file so that it can return the address
 *              of the staticly declared class struct.
 *
 * Returns:     SUCCEED (Can't fail)
 *
 *---------------------------------------------------------------------------
 */
herr_t
H5VL__native_introspect_get_conn_cls(void H5_ATTR_UNUSED *obj,
    H5VL_get_conn_lvl_t H5_ATTR_UNUSED lvl, const H5VL_class_t **conn_cls)
{
    FUNC_ENTER_PACKAGE_NOERR

    /* Sanity check */
    HDassert(conn_cls);

    /* Retrieve the native VOL connector class */
    *conn_cls = &H5VL_native_cls_g;

    FUNC_LEAVE_NOAPI(SUCCEED)
} /* end H5VL__native_introspect_get_conn_cls() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_native_get_file_addr_len
 *
 * Purpose:     Convenience function to get a file's address length from a
 *              location ID. Useful when you have to encode/decode addresses
 *              to/from tokens.
 *
 * Return:      Success:    Non-negative
 *              Failure:    Negative
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5VL_native_get_file_addr_len(hid_t loc_id, size_t *addr_len)
{
    H5VL_object_t   *vol_obj        = NULL;             /* Object token of loc_id */
    H5I_type_t      vol_obj_type    = H5I_BADID;        /* Object type of loc_id */
    hid_t           file_id         = H5I_INVALID_HID;  /* File ID */
    void            *vol_obj_file   = NULL;             /* Object token of file_id */
    H5F_t           *file           = NULL;             /* File stuct pointer */
    herr_t          ret_value       = SUCCEED;          /* Return value */

    FUNC_ENTER_NOAPI(FAIL)

    /* check arguments */
    HDassert(addr_len);

    /* Get object type */
    if((vol_obj_type = H5I_get_type(loc_id)) < 0)
        HGOTO_ERROR(H5E_VOL, H5E_BADTYPE, FAIL, "invalid location identifier")

    if (H5I_FILE == vol_obj_type) {
        /* Retrieve VOL object */
        if(NULL == (vol_obj_file = H5VL_vol_object(loc_id)))
            HGOTO_ERROR(H5E_VOL, H5E_BADTYPE, FAIL, "invalid location identifier")
    }
    else {
        /* Get the location object */
        if(NULL == (vol_obj = (H5VL_object_t *)H5I_object(loc_id)))
            HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, H5I_INVALID_HID, "invalid location identifier")

        /* Get the file for the object */
        if((file_id = H5F_get_file_id(vol_obj, vol_obj_type, FALSE)) < 0)
            HGOTO_ERROR(H5E_VOL, H5E_BADTYPE, FAIL, "not a file or file object")

        /* Retrieve VOL object */
        if(NULL == (vol_obj_file = H5VL_vol_object(file_id)))
            HGOTO_ERROR(H5E_VOL, H5E_BADTYPE, FAIL, "invalid location identifier")
    }

    /* Retrieve file struct from VOL object */
    if(NULL == (file = (H5F_t *)H5VL_object_data(vol_obj_file)))
        HGOTO_ERROR(H5E_VOL, H5E_BADTYPE, FAIL, "invalid VOL object")

    *addr_len = H5F_SIZEOF_ADDR(file);

done:
    if(file_id != H5I_INVALID_HID && H5I_dec_ref(file_id) < 0)
        HDONE_ERROR(H5E_VOL, H5E_CANTDEC, H5I_INVALID_HID, "unable to decrement refcount on file")
    FUNC_LEAVE_NOAPI(ret_value)
} /* end H5VL_native_get_file_addr_len() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_addr_to_token
 *
 * Purpose:     Converts a native VOL haddr_t address to an abstract VOL token.
 *
 * Return:      Success:    Non-negative
 *              Failure:    Negative
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5VL_addr_to_token(hid_t loc_id, haddr_t addr, H5VL_token_t *token)
{
    size_t          addr_len = 0;                   /* Size of haddr_t      */
    uint8_t         *p = (uint8_t *)token;          /* Pointer into token   */
    herr_t          ret_value = SUCCEED;            /* Return value         */

    FUNC_ENTER_NOAPI(FAIL)

    /* Check args */
    HDassert(token);

    /* Get the length of an haddr_t in the file */
    if(H5VL_native_get_file_addr_len(loc_id, &addr_len) < 0)
        HGOTO_ERROR(H5E_VOL, H5E_CANTGET, FAIL, "couldn't get length of haddr_t from loc_id")

    /* Encode token */
    H5F_addr_encode_len(addr_len, &p, addr);

done:
    FUNC_LEAVE_NOAPI(ret_value)
} /* end H5VL_addr_to_token() */


/*-------------------------------------------------------------------------
 * Function:    H5VL_token_to_addr
 *
 * Purpose:     Converts an abstract VOL token to a native VOL haddr_t address.
 *
 * Return:      Success:    Non-negative
 *              Failure:    Negative
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5VL_token_to_addr(hid_t loc_id, H5VL_token_t token, haddr_t *addr)
{
    size_t          addr_len = 0;                   /* Size of haddr_t      */
    const uint8_t   *p = (const uint8_t *)(&token); /* Pointer into token   */
    herr_t          ret_value = SUCCEED;            /* Return value         */

    FUNC_ENTER_NOAPI(FAIL)

    /* Check args */
    HDassert(addr);

    /* Get the length of an haddr_t in the file */
    if(H5VL_native_get_file_addr_len(loc_id, &addr_len) < 0)
        HGOTO_ERROR(H5E_VOL, H5E_CANTGET, FAIL, "couldn't get length of haddr_t from loc_id")

    /* Decode token */
    H5F_addr_decode_len(addr_len, &p, addr);

done:
    FUNC_LEAVE_NOAPI(ret_value)
} /* end H5VL_token_to_addr() */

