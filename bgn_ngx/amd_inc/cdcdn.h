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

/*Data Node*/

#ifndef _CDCDN_H
#define _CDCDN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

#include "type.h"

#include "cdcpgrb.h"
#include "cdcpgb.h"
#include "cdcpgd.h"
#include "cdcpgv.h"

#include "cmmap.h"

#define CDCDN_MEM_ALIGNMENT             (UINT32_ONE << 20) /*1MB alignment*/

#define CDCDN_AIO_TIMEOUT_NSEC          (60) /*seconds*/
#define CDCDN_FILE_AIO_TIMEOUT_NSEC     (30) /*seconds*/

#define CDCDN_032M_NODE_SIZE_NBITS      (25)
#define CDCDN_064M_NODE_SIZE_NBITS      (26)
#define CDCDN_128M_NODE_SIZE_NBITS      (27)
#define CDCDN_256M_NODE_SIZE_NBITS      (28)
#define CDCDN_512M_NODE_SIZE_NBITS      (29)
#define CDCDN_001G_NODE_SIZE_NBITS      (30)
#define CDCDN_002G_NODE_SIZE_NBITS      (31)
#define CDCDN_004G_NODE_SIZE_NBITS      (32)

#define CDCDN_032M_NODE                 (1)
#define CDCDN_064M_NODE                 (2)
#define CDCDN_128M_NODE                 (3)
#define CDCDN_256M_NODE                 (4)
#define CDCDN_512M_NODE                 (5)
#define CDCDN_001G_NODE                 (6)
#define CDCDN_002G_NODE                 (7)
#define CDCDN_004G_NODE                 (8)

//#define CDCDN_NODE_CHOICE               (CDCDN_032M_NODE)
//#define CDCDN_NODE_CHOICE               (CDCDN_064M_NODE)
//#define CDCDN_NODE_CHOICE               (CDCDN_128M_NODE)
//#define CDCDN_NODE_CHOICE               (CDCDN_256M_NODE)
//#define CDCDN_NODE_CHOICE               (CDCDN_512M_NODE)
//#define CDCDN_NODE_CHOICE               (CDCDN_001G_NODE)
//#define CDCDN_NODE_CHOICE               (CDCDN_002G_NODE)
//#define CDCDN_NODE_CHOICE               (CDCDN_004G_NODE)

#if (CDCDN_032M_NODE == CDCDN_NODE_CHOICE)
#define CDCDN_NODE_SIZE_NBITS           (CDCDN_032M_NODE_SIZE_NBITS)
#define CDCDN_NODE_DESC                 ("32M-node")
#endif/*(CDCDN_032M_NODE == CDCDN_NODE_CHOICE)*/

#if (CDCDN_064M_NODE == CDCDN_NODE_CHOICE)
#define CDCDN_NODE_SIZE_NBITS           (CDCDN_064M_NODE_SIZE_NBITS)
#define CDCDN_NODE_DESC                 ("64M-node")
#endif/*(CDCDN_064M_NODE == CDCDN_NODE_CHOICE)*/

#if (CDCDN_128M_NODE == CDCDN_NODE_CHOICE)
#define CDCDN_NODE_SIZE_NBITS           (CDCDN_128M_NODE_SIZE_NBITS)
#define CDCDN_NODE_DESC                 ("128M-node")
#endif/*(CDCDN_128M_NODE == CDCDN_NODE_CHOICE)*/

#if (CDCDN_256M_NODE == CDCDN_NODE_CHOICE)
#define CDCDN_NODE_SIZE_NBITS           (CDCDN_256M_NODE_SIZE_NBITS)
#define CDCDN_NODE_DESC                 ("256M-node")
#endif/*(CDCDN_256M_NODE == CDCDN_NODE_CHOICE)*/

#if (CDCDN_512M_NODE == CDCDN_NODE_CHOICE)
#define CDCDN_NODE_SIZE_NBITS           (CDCDN_512M_NODE_SIZE_NBITS)
#define CDCDN_NODE_DESC                 ("512M-node")
#endif/*(CDCDN_512M_NODE == CDCDN_NODE_CHOICE)*/

#if (CDCDN_001G_NODE == CDCDN_NODE_CHOICE)
#define CDCDN_NODE_SIZE_NBITS           (CDCDN_001G_NODE_SIZE_NBITS)
#define CDCDN_NODE_DESC                 ("1G-node")
#endif/*(CDCDN_001G_NODE == CDCDN_NODE_CHOICE)*/

#if (CDCDN_002G_NODE == CDCDN_NODE_CHOICE)
#define CDCDN_NODE_SIZE_NBITS           (CDCDN_002G_NODE_SIZE_NBITS)
#define CDCDN_NODE_DESC                 ("2G-node")
#endif/*(CDCDN_002G_NODE == CDCDN_NODE_CHOICE)*/

#if (CDCDN_004G_NODE == CDCDN_NODE_CHOICE)
#define CDCDN_NODE_SIZE_NBITS           (CDCDN_004G_NODE_SIZE_NBITS)
#define CDCDN_NODE_DESC                 ("4G-node")
#endif/*(CDCDN_004G_NODE == CDCDN_NODE_CHOICE)*/

#define CDCDN_NODE_SIZE_NBYTES          ((UINT32)(UINT32_ONE << CDCDN_NODE_SIZE_NBITS))
#define CDCDN_SEG_NO_NBITS              (CDCDN_NODE_SIZE_NBITS - CDCPGB_SIZE_NBITS) /*how many blocks in one node*/
#define CDCDN_SEG_NO_MASK               (((UINT32)(UINT32_ONE << CDCDN_SEG_NO_NBITS)) - 1)

/*node id = disk_no | block_no => node id is the block id in whole space*/

#define CDCDN_NODE_ID_MAKE(disk_no, block_no)           ((((UINT32)(disk_no)) << (CDCPGD_SIZE_NBITS - CDCPGB_SIZE_NBITS)) | (((UINT32)(block_no)) << 0))
#define CDCDN_NODE_ERR_ID                               (CDCDN_NODE_ID_MAKE(CDCPGRB_ERR_POS, CDCPGRB_ERR_POS))
#define CDCDN_NODE_ERR_OFFSET                           ((UINT32)~0)

/*seg no is the block id in this node*/
#define CDCDN_NODE_ID_GET_SEG_NO(node_id)               ((uint16_t)(((node_id) >>  0) & CDCDN_SEG_NO_MASK))

typedef struct
{
    int                ssd_meta_fd;
    int                ssd_disk_fd;

    uint32_t           read_only_flag   :1;/*dn is read-only if set*/
    uint32_t           dontdump_flag    :1;/*dn not flush or dump if set*/
    uint32_t           rsvd01           :30;
    uint32_t           rsvd02;

    UINT32             node_num;

    UINT32             base_s_offset;      /*start offset of data node header (cdcpgv_header)*/
    UINT32             base_e_offset;      /*end offset of data node header (cdcpgv_header)*/

    /*disk storage range: [start, end)*/
    UINT32             node_s_offset;      /*start offset of disk storage*/
    UINT32             node_e_offset;      /*end offset of disk storage. end = start + len*/

    CDCPGV            *cdcpgv;
}CDCDN;

#define CDCDN_RDONLY_FLAG(cdcdn)                         ((cdcdn)->read_only_flag)
#define CDCDN_DONTDUMP_FLAG(cdcdn)                       ((cdcdn)->dontdump_flag)
#define CDCDN_NODE_SSD_META_FD(cdcdn)                    ((cdcdn)->ssd_meta_fd)
#define CDCDN_NODE_SSD_DISK_FD(cdcdn)                    ((cdcdn)->ssd_disk_fd)
#define CDCDN_NODE_NUM(cdcdn)                            ((cdcdn)->node_num)
#define CDCDN_BASE_S_OFFSET(cdcdn)                       ((cdcdn)->base_s_offset)
#define CDCDN_BASE_E_OFFSET(cdcdn)                       ((cdcdn)->base_e_offset)
#define CDCDN_NODE_S_OFFSET(cdcdn)                       ((cdcdn)->node_s_offset)
#define CDCDN_NODE_E_OFFSET(cdcdn)                       ((cdcdn)->node_e_offset)
#define CDCDN_CDCPGV(cdcdn)                              ((cdcdn)->cdcpgv)

#define CDCDN_OPEN_NODE(_cdcdn, node_id)                 (cdcdn_node_fetch((_cdcdn), (node_id)))

UINT32 cdcdn_node_fetch(const CDCDN *cdcdn, const UINT32 node_id);

UINT32 cdcdn_node_locate(CDCDN *cdcdn, const uint16_t disk_no, const uint16_t block_no, const uint16_t page_no);

EC_BOOL cdcdn_node_write(CDCDN *cdcdn, const UINT32 node_id, const UINT32 data_max_len, const UINT8 *data_buff, UINT32 *offset);

EC_BOOL cdcdn_node_read(CDCDN *cdcdn, const UINT32 node_id, const UINT32 data_max_len, UINT8 *data_buff, UINT32 *offset);

EC_BOOL cdcdn_compute_disk(const UINT32 ssd_disk_size, UINT32 *disk_num, UINT32 *node_num, UINT32 *block_num);

EC_BOOL cdcdn_compute_meta(const UINT32 ssd_disk_size, UINT32 *disk_num, UINT32 *node_num, UINT32 *block_num);

CDCDN *cdcdn_create(UINT32 *meta_s_offset, const UINT32 meta_e_offset,
                        const UINT32 data_s_offset, const UINT32 data_e_offset,
                        const uint16_t disk_num, const UINT32 node_num, const UINT32 block_num);

CDCDN *cdcdn_create_shm(CMMAP_NODE *cmmap_node, UINT32 *meta_s_offset, const UINT32 meta_e_offset,
                                const UINT32 data_s_offset, const UINT32 data_e_offset,
                                const uint16_t disk_num, const UINT32 node_num, const UINT32 block_num);

EC_BOOL cdcdn_add_disk(CDCDN *cdcdn, const uint16_t disk_no, UINT8 *base, UINT32 *pos);

EC_BOOL cdcdn_del_disk(CDCDN *cdcdn, const uint16_t disk_no);

CDCDN *cdcdn_new();

EC_BOOL cdcdn_init(CDCDN *cdcdn);

EC_BOOL cdcdn_clean(CDCDN *cdcdn);

EC_BOOL cdcdn_free(CDCDN *cdcdn);

EC_BOOL cdcdn_close(CDCDN *cdcdn);

EC_BOOL cdcdn_set_read_only(CDCDN *cdcdn);

EC_BOOL cdcdn_unset_read_only(CDCDN *cdcdn);

EC_BOOL cdcdn_is_read_only(const CDCDN *cdcdn);

EC_BOOL cdcdn_set_dontdump(CDCDN *cdcdn);

EC_BOOL cdcdn_unset_dontdump(CDCDN *cdcdn);

EC_BOOL cdcdn_is_dontdump(const CDCDN *cdcdn);

void cdcdn_print(LOG *log, const CDCDN *cdcdn);

REAL cdcdn_used_ratio(const CDCDN *cdcdn);

EC_BOOL cdcdn_is_full(CDCDN *cdcdn);

/*random access for reading, the offset is for the whole 64M page-block */
EC_BOOL cdcdn_read_o(CDCDN *cdcdn, const uint16_t disk_no, const uint16_t block_no, const UINT32 offset, const UINT32 data_max_len, UINT8 *data_buff, UINT32 *data_len);

/*random access for writting */
/*offset: IN/OUT, the offset is for the whole 64M page-block*/
EC_BOOL cdcdn_write_o(CDCDN *cdcdn, const UINT32 data_max_len, const UINT8 *data_buff, const uint16_t disk_no, const uint16_t block_no, UINT32 *offset);

/*random access for reading, the offset is for the user file but not for the whole 64M page-block */
EC_BOOL cdcdn_read_e(CDCDN *cdcdn, const uint16_t disk_no, const uint16_t block_no, const uint16_t page_no, const uint32_t offset, const UINT32 data_max_len, UINT8 *data_buff, UINT32 *data_len);

/*random access for writting, the offset is for the user file but not for the whole 64M page-block */
EC_BOOL cdcdn_write_e(CDCDN *cdcdn, const UINT32 data_max_len, const UINT8 *data_buff, const uint16_t disk_no, const uint16_t block_no, const uint16_t page_no, const uint32_t offset);

EC_BOOL cdcdn_read_p(CDCDN *cdcdn, const uint16_t disk_no, const uint16_t block_no, const uint16_t page_no, const UINT32 data_max_len, UINT8 *data_buff, UINT32 *data_len);

EC_BOOL cdcdn_write_p(CDCDN *cdcdn, const UINT32 data_max_len, const UINT8 *data_buff, uint16_t *disk_no, uint16_t *block_no, uint16_t *page_no);

EC_BOOL cdcdn_flush(CDCDN *cdcdn);

EC_BOOL cdcdn_load(CDCDN *cdcdn, int fd, UINT32 *s_offset, const UINT32 e_offset);

EC_BOOL cdcdn_load_shm(CDCDN *cdcdn, CMMAP_NODE *cmmap_node, int fd, UINT32 *s_offset, const UINT32 e_offset);

EC_BOOL cdcdn_retrieve_shm(CDCDN *cdcdn, CMMAP_NODE *cmmap_node, int fd, UINT32 *s_offset, const UINT32 e_offset);

#endif/* _CDCDN_H */

#ifdef __cplusplus
}
#endif/*__cplusplus*/

