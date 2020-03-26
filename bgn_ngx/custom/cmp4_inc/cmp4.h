/******************************************************************************
*
* Copyright (C) Chaoyong Zhou
* Email: bgnvendor@163.com
* QQ: 2796796
*
*******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif/*__cplusplus*/

#if (SWITCH_ON == NGX_BGN_SWITCH)

#ifndef _CMP4_H
#define _CMP4_H

#include "type.h"
#include "cstring.h"
#include "clist.h"

#include "csocket.h"
#include "mod.inc"

#include "cngx.h"
#include "cngx_mp4.h"

#include "chttp.h"

#include "cbytes.h"
#include "crange.h"

#define CMP4_MODULE_NAME           ("cmp4")

#define CMP4_ERR_SEG_NO            ((UINT32)~0)

#define CMP4_BUFFER_SIZE_DEFAULT            (512 * 1024)
#define CMP4_MAX_BUFFER_SIZE_DEFAULT        (10 * 1024 * 1024)

typedef struct
{
    /* used counter >= 0 */
    UINT32               usedcounter;
    EC_BOOL              terminate_flag;

    UINT32               cache_seg_max_num;
    UINT32               cache_seg_size;
    CSTRING              cache_path;
    const char          *cache_status;  /*TCP_HIT, TCP_MISS, TCP_REFRESH_HIT, TCP_REFRESH_MISS*/

    ngx_http_request_t  *ngx_http_req;
    CNGX_OPTION          cngx_option;

    /*--- parse from cngx http request ---*/
    uint32_t             cngx_debug_switch_on_flag          :1; /*if debug mode indicated in cngx http req*/
    uint32_t             cngx_range_exist_flag              :1; /*exist field 'Range' in request header*/
    uint32_t             cngx_range_multiple_flag           :1; /*multiple ranges in request header*/
    uint32_t             cngx_range_adjusted_flag           :1; /*if range is adjust or split*/
    uint32_t             cngx_range_filtered_flag           :1; /*if range is adjust or split*/
    uint32_t             cngx_range_start_zero_endless_flag :1; /*range is "0-"*/
    uint32_t             cngx_direct_ims_flag               :1; /*direct orig when ims miss and switch is on*/
    uint32_t             cache_expired_flag                 :1; /*if cache is expired*/
    uint32_t             content_length_exist_flag          :1; /*exist field 'Content-Length' in response header*/
    uint32_t             orig_force_flag                    :1; /*force to orig*/
    uint32_t             orig_no_cache_flag                 :1; /*orig indicate no-cache or 404 etc*/
    uint32_t             cngx_preload_switch_on_flag        :1; /*cngx http PUT req indicate preload mode*/
    uint32_t             rsvd01                             :20;
    uint32_t             rsvd02;

    CRANGE_MGR           cngx_range_mgr;

    UINT32               content_length;
    CSTRING              cached_etag;
    CSTRING              cached_last_modified;

    CSTRING              header_last_modified;
    CSTRING              header_etag;

    cngx_mp4_file_t     *mp4;
    UINT32               mp4_start;               /*for MP4*/
    UINT32               mp4_length;              /*for MP4*/

    /*---- debug ----*/
    UINT32               depth;                   /*recursive depth*/

    CHTTP_REQ           *chttp_req;
    CHTTP_RSP           *chttp_rsp;
    CHTTP_STORE         *chttp_store;
    CHTTP_STAT          *chttp_stat;

    UINT32               absent_seg_no;
    UINT32               sent_body_size;

    UINT32               ngx_loc;  /*ngx rc report at location*/
    ngx_int_t            ngx_rc;   /*save ngx calling result*/
}CMP4_MD;

#define CMP4_MD_TERMINATE_FLAG(cmp4_md)                     ((cmp4_md)->terminate_flag)

#define CMP4_MD_CACHE_SEG_MAX_NUM(cmp4_md)                  ((cmp4_md)->cache_seg_max_num)
#define CMP4_MD_CACHE_SEG_SIZE(cmp4_md)                     ((cmp4_md)->cache_seg_size)
#define CMP4_MD_CACHE_PATH(cmp4_md)                         (&((cmp4_md)->cache_path))
#define CMP4_MD_CACHE_STATUS(cmp4_md)                       ((cmp4_md)->cache_status)

#define CMP4_MD_NGX_HTTP_REQ(cmp4_md)                       ((cmp4_md)->ngx_http_req)
#define CMP4_MD_CNGX_OPTION(cmp4_md)                        (&((cmp4_md)->cngx_option))

#define CMP4_MD_CNGX_DEBUG_SWITCH_ON_FLAG(cmp4_md)          ((cmp4_md)->cngx_debug_switch_on_flag)
#define CMP4_MD_CNGX_PRELOAD_SWITCH_ON_FLAG(cmp4_md)        ((cmp4_md)->cngx_preload_switch_on_flag)
#define CMP4_MD_CNGX_RANGE_EXIST_FLAG(cmp4_md)              ((cmp4_md)->cngx_range_exist_flag)
#define CMP4_MD_CNGX_RANGE_MULTIPLE_FLAG(cmp4_md)           ((cmp4_md)->cngx_range_multiple_flag)
#define CMP4_MD_CNGX_RANGE_ADJUSTED_FLAG(cmp4_md)           ((cmp4_md)->cngx_range_adjusted_flag)
#define CMP4_MD_CNGX_RANGE_FILTERED_FLAG(cmp4_md)           ((cmp4_md)->cngx_range_filtered_flag)
#define CMP4_MD_CNGX_RANGE_START_ZERO_ENDLESS_FLAG(cmp4_md) ((cmp4_md)->cngx_range_start_zero_endless_flag)
#define CMP4_MD_CNGX_DIRECT_IMS_FLAG(cmp4_md)               ((cmp4_md)->cngx_direct_ims_flag)
#define CMP4_MD_CACHE_EXPIRED_FLAG(cmp4_md)                 ((cmp4_md)->cache_expired_flag)
#define CMP4_MD_CONTENT_LENGTH_EXIST_FLAG(cmp4_md)          ((cmp4_md)->content_length_exist_flag)
#define CMP4_MD_ORIG_FORCE_FLAG(cmp4_md)                    ((cmp4_md)->orig_force_flag)
#define CMP4_MD_ORIG_NO_CACHE_FLAG(cmp4_md)                 ((cmp4_md)->orig_no_cache_flag)

#define CMP4_MD_CNGX_RANGE_MGR(cmp4_md)                     (&((cmp4_md)->cngx_range_mgr))

#define CMP4_MD_CONTENT_LENGTH(cmp4_md)                     ((cmp4_md)->content_length)
#define CMP4_MD_CACHED_ETAG(cmp4_md)                        (&((cmp4_md)->cached_etag))
#define CMP4_MD_CACHED_LAST_MODIFED(cmp4_md)                (&((cmp4_md)->cached_last_modified))

#define CMP4_MD_HEADER_LAST_MODIFIED(cmp4_md)               (&((cmp4_md)->header_last_modified))
#define CMP4_MD_HEADER_ETAG(cmp4_md)                        (&((cmp4_md)->header_etag))

#define CMP4_MD_MP4(cmp4_md)                                ((cmp4_md)->mp4)
#define CMP4_MD_MP4_START(cmp4_md)                          ((cmp4_md)->mp4_start)
#define CMP4_MD_MP4_LENGTH(cmp4_md)                         ((cmp4_md)->mp4_length)

/*shortcut*/
#define CMP4_MD_MP4_OUT_CHAIN(cmp4_md)   \
    (NULL_PTR == CMP4_MD_MP4(cmp4_md)? NULL_PTR : CMP4_MD_MP4(cmp4_md)->out)

#define CMP4_MD_DEPTH(cmp4_md)                              ((cmp4_md)->depth)

#define CMP4_MD_CHTTP_REQ(cmp4_md)                          ((cmp4_md)->chttp_req)
#define CMP4_MD_CHTTP_RSP(cmp4_md)                          ((cmp4_md)->chttp_rsp)
#define CMP4_MD_CHTTP_STORE(cmp4_md)                        ((cmp4_md)->chttp_store)
#define CMP4_MD_CHTTP_STAT(cmp4_md)                         ((cmp4_md)->chttp_stat)
#define CMP4_MD_ABSENT_SEG_NO(cmp4_md)                      ((cmp4_md)->absent_seg_no)
#define CMP4_MD_SENT_BODY_SIZE(cmp4_md)                     ((cmp4_md)->sent_body_size)

#define CMP4_MD_NGX_LOC(cmp4_md)                            ((cmp4_md)->ngx_loc)
#define CMP4_MD_NGX_RC(cmp4_md)                             ((cmp4_md)->ngx_rc)

#define CMP4_MD_SEND_BODY_PRELOAD_FLAG(cmp4_md) \
    ((BIT_FALSE == CMP4_MD_CNGX_PRELOAD_SWITCH_ON_FLAG(cmp4_md)) ? 0 : CNGX_SEND_BODY_PRELOAD_FLAG)


/**
*   for test only
*
*   to query the status of CMP4 Module
*
**/
void cmp4_print_module_status(const UINT32 cmp4_md_id, LOG *log);

/**
*
* register CMP4 module
*
**/
EC_BOOL cmp4_reg();

/**
*
* unregister CMP4 module
*
**/
EC_BOOL cmp4_unreg();

/**
*
* start CMP4 module
*
**/
UINT32 cmp4_start(ngx_http_request_t *r);

/**
*
* end CMP4 module
*
**/
void cmp4_end(const UINT32 cmp4_md_id);

EC_BOOL cmp4_get_ngx_rc(const UINT32 cmp4_md_id, ngx_int_t *rc, UINT32 *location);

/*only for failure!*/
EC_BOOL cmp4_set_ngx_rc(const UINT32 cmp4_md_id, const ngx_int_t rc, const UINT32 location);

/*only for failure!*/
EC_BOOL cmp4_override_ngx_rc(const UINT32 cmp4_md_id, const ngx_int_t rc, const UINT32 location);

EC_BOOL cmp4_get_cache_seg_uri(const UINT32 cmp4_md_id, const UINT32 seg_no, CSTRING *cache_uri);

EC_BOOL cmp4_get_cache_seg(const UINT32 cmp4_md_id, const UINT32 seg_no, CBYTES *seg_cbytes);

EC_BOOL cmp4_get_cache_seg_n(const UINT32 cmp4_md_id, const CRANGE_SEG *crange_seg, CBYTES *seg_cbytes);

EC_BOOL cmp4_get_req_range_segs(const UINT32 cmp4_md_id, const UINT32 seg_size);

EC_BOOL cmp4_get_rsp_length_segs(const UINT32 cmp4_md_id, const UINT32 seg_size);

EC_BOOL cmp4_filter_rsp_range(const UINT32 cmp4_md_id);

EC_BOOL cmp4_filter_header_in_common(const UINT32 cmp4_md_id, CHTTP_REQ *chttp_req);

EC_BOOL cmp4_filter_header_out_common(const UINT32 cmp4_md_id, const char *procedure);

EC_BOOL cmp4_filter_header_out_cache_control(const UINT32 cmp4_md_id);

EC_BOOL cmp4_filter_header_out_single_range(const UINT32 cmp4_md_id);

EC_BOOL cmp4_filter_header_out_range(const UINT32 cmp4_md_id);

EC_BOOL cmp4_renew_header_cache(const UINT32 cmp4_md_id, const char *k, const char *v);

EC_BOOL cmp4_content_handler(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_head_header_in_filter_upstream(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_head_header_in_filter_server(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_head_header_in_filter_host(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_head_header_in_filter_port(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_head_header_in_filter_ipaddr(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_head_header_in_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_head_header_out_rsp_status_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_head_header_out_connection_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_head_header_out_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_head_send_request(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_head_send_response(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_head_procedure(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_header_in_filter_upstream(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_header_in_filter_server(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_header_in_filter_host(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_header_in_filter_port(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_header_in_filter_ipaddr(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_header_in_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_header_out_length_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_header_out_range_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_header_out_rsp_status_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_header_out_connection_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_header_out_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_body_out_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_set_store(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_send_request(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_send_seg_n(const UINT32 cmp4_md_id, const CRANGE_SEG *crange_seg);

EC_BOOL cmp4_content_direct_send_node(const UINT32 cmp4_md_id, CRANGE_NODE *crange_node);

EC_BOOL cmp4_content_direct_import_header(const UINT32 cmp4_md_id, const CHTTP_NODE *chttp_node);

EC_BOOL cmp4_content_direct_send_header(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_send_body(const UINT32 cmp4_md_id, const UINT32 seg_no, const UINT8 *data, const UINT32 len);

EC_BOOL cmp4_content_direct_send_response(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_direct_procedure(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair2_header_in_filter_upstream(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair2_header_in_filter_server(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair2_header_in_filter_host(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair2_header_in_filter_port(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair2_header_in_filter_ipaddr(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair2_header_in_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair2_header_out_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair2_body_out_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair2_send_request(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair2_send_response(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair2_procedure(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_orig_header_in_filter_upstream(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_orig_header_in_filter_server(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_orig_header_in_filter_host(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_orig_header_in_filter_port(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_orig_header_in_filter_ipaddr(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_orig_header_in_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_orig_header_out_range_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_orig_header_out_rsp_status_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_orig_header_out_cache_control_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_orig_header_out_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_orig_body_out_filter(const UINT32 cmp4_md_id, const UINT32 seg_no, uint8_t **data, uint32_t *len);

EC_BOOL cmp4_content_orig_set_store(const UINT32 cmp4_md_id, CHTTP_STORE *chttp_store);

EC_BOOL cmp4_content_orig_send_request(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_orig_send_seg_n(const UINT32 cmp4_md_id, const CRANGE_SEG *crange_seg);

EC_BOOL cmp4_content_orig_send_response(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_orig_procedure(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_header_in_filter_upstream(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_header_in_filter_server(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_header_in_filter_host(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_header_in_filter_port(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_header_in_filter_ipaddr(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_header_in_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_header_out_if_modified_since_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_header_out_if_none_match_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_header_out_range_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_header_out_rsp_status_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_header_out_cache_control_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_header_out_connection_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_header_out_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_body_out_filter(const UINT32 cmp4_md_id, const UINT32 seg_no, uint8_t **data, uint32_t *len);

EC_BOOL cmp4_content_ms_set_store(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_send_request(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_send_seg_n(const UINT32 cmp4_md_id, const CRANGE_SEG *crange_seg);

EC_BOOL cmp4_content_ms_import_header(const UINT32 cmp4_md_id, const CHTTP_NODE *chttp_node);

EC_BOOL cmp4_content_ms_send_header(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_send_body(const UINT32 cmp4_md_id, const UINT32 seg_no, const UINT8 *data, const UINT32 len);

EC_BOOL cmp4_content_ms_send_response(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ms_procedure(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_preload_parse_header(const UINT32 cmp4_md_id, const CBYTES *header_cbytes);

EC_BOOL cmp4_content_preload_header_out_range_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_preload_header_out_rsp_status_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_preload_header_out_mp4_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_preload_header_out_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_preload_send_response(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_preload_procedure(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_redirect_procedure(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ims_header_in_filter_upstream(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ims_header_in_filter_server(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ims_header_in_filter_host(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ims_header_in_filter_port(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ims_header_in_filter_ipaddr(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ims_header_in_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ims_header_out_304_last_modified_filter(const UINT32 cmp4_md_id, const UINT32 cmp4_md_id_t, uint32_t status);

EC_BOOL cmp4_content_ims_header_out_304_expires_filter(const UINT32 cmp4_md_id, const UINT32 cmp4_md_id_t, uint32_t status);

EC_BOOL cmp4_content_ims_header_out_304_date_filter(const UINT32 cmp4_md_id, const UINT32 cmp4_md_id_t, uint32_t status);

EC_BOOL cmp4_content_ims_header_out_304_age_filter(const UINT32 cmp4_md_id, const UINT32 cmp4_md_id_t, uint32_t status);

EC_BOOL cmp4_content_ims_header_out_304_content_range_filter(const UINT32 cmp4_md_id, const UINT32 cmp4_md_id_t, uint32_t status);

EC_BOOL cmp4_content_ims_header_out_304_filter(const UINT32 cmp4_md_id, const UINT32 cmp4_md_id_t, uint32_t status);

EC_BOOL cmp4_content_ims_header_out_not_304_last_modified_filter(const UINT32 cmp4_md_id, const UINT32 cmp4_md_id_t, uint32_t status);

EC_BOOL cmp4_content_ims_header_out_not_304_etag_filter(const UINT32 cmp4_md_id, const UINT32 cmp4_md_id_t, uint32_t status);

EC_BOOL cmp4_content_ims_header_out_not_304_expires_filter(const UINT32 cmp4_md_id, const UINT32 cmp4_md_id_t, uint32_t status);

EC_BOOL cmp4_content_ims_header_out_not_304_content_range_filter(const UINT32 cmp4_md_id, const UINT32 cmp4_md_id_t, uint32_t status);

EC_BOOL cmp4_content_ims_header_out_not_304_filter(const UINT32 cmp4_md_id, const UINT32 cmp4_md_id_t, uint32_t status);

EC_BOOL cmp4_content_ims_send_request(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_ims_procedure(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair_header_in_filter_upstream(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair_header_in_filter_server(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair_header_in_filter_host(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair_header_in_filter_port(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair_header_in_filter_ipaddr(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair_header_in_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair_send_request(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_repair_fetch_response(const UINT32 cmp4_md_id, const CRANGE_SEG *crange_seg, CBYTES *seg_cbytes);

EC_BOOL cmp4_content_repair_procedure(const UINT32 cmp4_md_id, const CRANGE_SEG *crange_seg, CBYTES *seg_cbytes);;

EC_BOOL cmp4_content_expired_header_out_range_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_expired_header_out_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_expired_body_out_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_expired_send_seg_n(const UINT32 cmp4_md_id, const CRANGE_SEG *crange_seg);

EC_BOOL cmp4_content_expired_send_node(const UINT32 cmp4_md_id, CRANGE_NODE *crange_node);

EC_BOOL cmp4_content_expired_send_response(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_expired_procedure(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_cache_parse_header(const UINT32 cmp4_md_id, const CBYTES *header_cbytes);

EC_BOOL cmp4_content_cache_save_header(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_cache_header_out_range_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_cache_header_out_rsp_status_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_cache_header_out_expires_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_cache_header_out_age_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_cache_header_out_cache_status_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_cache_header_out_filter(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_cache_body_out_filter(const UINT32 cmp4_md_id, const UINT32 seg_no, uint8_t **data, uint32_t *len);

EC_BOOL cmp4_content_cache_send_seg_n(const UINT32 cmp4_md_id, const CRANGE_SEG *crange_seg);

EC_BOOL cmp4_content_cache_send_node(const UINT32 cmp4_md_id, CRANGE_NODE *crange_node);

EC_BOOL cmp4_content_cache_send_response(const UINT32 cmp4_md_id);

EC_BOOL cmp4_content_cache_procedure(const UINT32 cmp4_md_id);

#endif /*_CMP4_H*/

#endif/*(SWITCH_ON == NGX_BGN_SWITCH)*/

#ifdef __cplusplus
}
#endif/*__cplusplus*/


