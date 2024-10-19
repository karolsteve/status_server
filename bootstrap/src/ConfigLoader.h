/*
 * @author: Steve Tchatchouang
 * Date : 12/10/24
 */
//

#ifndef TKS_GATEWAY_CONFIG_LOADER_H
#define TKS_GATEWAY_CONFIG_LOADER_H

#include "StatusConfig.h"

class ConfigLoader {
public:
    static StatusConfig load_config(const std::string& file_path);
};


#endif //TKS_GATEWAY_CONFIG_LOADER_H
