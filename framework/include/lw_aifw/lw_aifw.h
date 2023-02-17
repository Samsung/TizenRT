 /****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/**
 * @file lw_aifw.h
 * @brief This file defines the structures to be filled by application and passed to Light Weight AI Framework for usage.
 */

#ifndef __LW_AIFW_H__
#define __LW_AIFW_H__

#include <tinyara/config.h>
#include "stdint.h"
#include "lw_aifw/lw_aifw_result.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NUMBER_OF_MODEL CONFIG_LW_AIFW_MAXM_MODEL_COUNT

/**
 * @brief Enum defines the model types accepted by Light Weight AI Framework.
 * Values other than below mentioned will lead to failure in LW AIFW initialization.
 */
typedef enum _LW_AIFW_MODEL_TYPE {
	ARRAY_MODEL = 0,
	FILE_MODEL = 1,
} LW_AIFW_MODEL_TYPE;

/**
 * @brief TFModel structure defines the properties of a model. All these values are filled by an application and passed to LW AIFW for usage.
 * ModelType which can any value of type LW_AIFW_MODEL_TYPE.
 * Model is a reference to AI model, AIModel class loads this model based on ModelType value.
 * CountInputVals: This defines the input of parameters received by model.
 */
struct TFModel {
		LW_AIFW_MODEL_TYPE ModelType;
		unsigned char *Model;
		uint32_t ModelCode;
		uint16_t CountInputVals;
};

/**
 * @brief AIAppMetadata structure is populated by AI application and passed to LW AIFW.
 * ModelCount Number of models supported by application.
 * Models Array of TFModel objects. LW AIFW will load all models during initialization. Maximum number of supported models is defined by MAX_NUMBER_OF_MODEL.
 * DataReadyListener: A callback function which is called when data is ready for pre-processing and inference.
 */
struct AIAppMetadata {
		uint16_t ModelCount;
		struct TFModel Models[MAX_NUMBER_OF_MODEL];
		void (*DataReadyListener)(LW_AIFW_RESULT);
};

#ifdef __cplusplus
}
#endif

#endif /* __LW_AIFW_H__ */

