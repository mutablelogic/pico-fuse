/** @file map.h
 *  @brief Private function prototypes and structure definitions for maps
 */
#ifndef FUSE_PRIVATE_MAP_H
#define FUSE_PRIVATE_MAP_H
#include <fuse/fuse.h>

/* @brief Represents an instance of a map
 */
struct fuse_map_instance
{
    size_t size;                 ///< The maximum number of nodes in the instance
    size_t count;                ///< The number of nodes in the instance
    struct fuse_map_node *nodes; ///< The nodes in the instance
};

/* @brief Represents a node in a map
 */
struct fuse_map_node
{
    void *key;   ///< The node key
    void *value; ///< The node value
};

#endif
