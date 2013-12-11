#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include <stdbool.h>
#include <stdint.h>
#include <uthash.h>

#define MMDBW_RECORD_TYPE_EMPTY (0)
#define MMDBW_RECORD_TYPE_DATA (1)
#define MMDBW_RECORD_TYPE_NODE (2)

typedef struct MMDBW_record_s {
    int type;
    union {
        uint8_t *md5_key;
        struct MMDBW_node_s *node;
    } value;
} MMDBW_record_s;

typedef struct MMDBW_node_s {
    /* Making this signed limits us to ((2**64)/2)-1 nodes per tree, but
     * hopefully that will be enough. */
    int64_t node_number;
    MMDBW_record_s left_record;
    MMDBW_record_s right_record;
    UT_hash_handle hh;
} MMDBW_node_s;

typedef struct MMDBW_data_s {
    SV *data;
    uint8_t *md5_key;
    UT_hash_handle hh;
} MMDBW_data_s;

typedef struct MMDBW_tree_s {
    uint8_t ip_version;
    uint8_t record_size;
    MMDBW_data_s *data_table;
    uint32_t nodes_per_alloc;
    MMDBW_node_s *root_node;
    MMDBW_node_s **node_pool;
    uint32_t next_node;
    uint32_t allocated_nodes;
    uint64_t node_count;
    bool is_finalized;
    SV *data_encoder;
} MMDBW_tree_s;

typedef struct MMDBW_network_s {
    uint8_t *bytes;
    uint8_t mask_length;
    uint8_t max_depth0;
    int family;
    int gai_status;
} MMDBW_network_s;

    /* *INDENT-OFF* */
    /* --prototypes automatically generated by dev-bin/regen-prototypes.pl - don't remove this comment */
    extern MMDBW_tree_s *new_tree(uint8_t ip_version, uint8_t record_size,
                                  uint32_t nodes_per_alloc);
    extern int insert_network(MMDBW_tree_s *tree, char *ipstr, uint8_t mask_length,
                              SV *key, SV *data);
    extern void delete_reserved_networks(MMDBW_tree_s *tree);
    extern void alias_ipv4_networks(MMDBW_tree_s *tree);
    extern MMDBW_node_s *new_node(MMDBW_tree_s *tree);
    extern void finalize_tree(MMDBW_tree_s *tree);
    extern void write_tree(MMDBW_tree_s *tree, FILE *fd, SV *encoder);
    extern char *md5_to_hex(uint8_t digest[16]);
    extern SV *data_for_key(MMDBW_tree_s *tree, uint8_t *md5_key);
    extern void free_tree(MMDBW_tree_s *tree);
    extern char *node_type_name(int node_type);
    extern void warn_hex(uint8_t digest[16], char *where);
    extern char *md5_as_hex(uint8_t digest[16]);
    /* --prototypes end - don't remove this comment-- */
    /* *INDENT-ON* */

