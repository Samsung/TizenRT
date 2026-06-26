/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ONERT_MICRO_H_
#define _ONERT_MICRO_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * typical training flow in onert-micro
 *
 * 1. load model or checkpoint
 *   1-1. (optional) configure training options
 * 2. feed training input / output(e.g. label) data (cf. unit of a step)
 * 3. train a step
 * 4. check loss
 *   4-0. save checkpoint for recovery/resume training
 *   4-1. no more traning -> go to 5
 *   4-2. more training -> go to 2
 * 5. save current state to inference model
 * 6. inference with inference model
// sample example
// 0. create context
nnfw_session *session;
nnfw_create_session(&session);
// 1. load model (and checkpoint if continue training)
nnfw_load_model_from_file(session, MODEL_PATH);
// 1-1. (optional, TBD) configure training options
nnfw_load_ckpt_from_file(session, CKPT_PATH);
nnfw_train_prepare(session);
float training_input[BATCH_SIZE*INPUT_SIZE];
float training_label[BATCH_SIZE*OUTPUT_SIZE];
// main training loop
for(int epoch=0; epoch < NUM_EPOCHS; epoch++) {
  for(int step=0; step < NUM_BATCHES ; step++) {
    // prepare this steps's intput/label
    memcpy(training_input, train_input_data + THIS_BATCH_OFFSET, BATCH_SIZE*INPUT_SIZE);
    memcpy(training_output, train_output_data + THIS_BATCH_OFFSET, BATCH_SIZE*OUTPUT_SIZE);
    // 2. feed training input / expected output
    nnfw_train_set_input(session, 0 , training_input, NULL);
    nnfw_train_set_expected(session, 0 , training_output, NULL);
    // 3. train a step
    nnfw_train(session);
  }
  // 4. check loss
  float loss;
  nnfw_train_get_loss(ctx, 0, &loss);
  if(loss > TARGET_LOSS) {
    nnfw_train_save_as_checkpoint(ctx, CKPT_PATH);
  }
  else {
    nnfw_train_export_circle(ctx, CIRCLE_PATH);
  }
}
*/

typedef struct nnfw_session nnfw_session;

typedef enum
{
  /** A tensor of 32 bit floating point */
  NNFW_TYPE_TENSOR_FLOAT32 = 0,
  /** A tensor of 32 bit signed integer */
  NNFW_TYPE_TENSOR_INT32 = 1,
} NNFW_TYPE;

/**
 * @brief Result values returned from a call to an API function
 */
typedef enum
{
  /** Successful */
  NNFW_STATUS_NO_ERROR = 0,
  /**
   * An error code for general use.
   * Mostly used when there is no specific value for that certain situation.
   */
  NNFW_STATUS_ERROR = 1,
  /** Unexpected null argument is given. */
  NNFW_STATUS_UNEXPECTED_NULL = 2,
  /** When a function was called but it is not valid for the current session state. */
  NNFW_STATUS_INVALID_STATE = 3,
  /** When it is out of memory */
  NNFW_STATUS_OUT_OF_MEMORY = 4,
  /** When it was given an insufficient output buffer */
  NNFW_STATUS_INSUFFICIENT_OUTPUT_SIZE = 5,
  /** When API is deprecated */
  NNFW_STATUS_DEPRECATED_API = 6,
} NNFW_STATUS;

/**
 * @brief Maximum rank expressible with nnfw
 */
#define NNFW_MAX_RANK (6)

/**
 * @brief tensor info describes the type and shape of tensors
 *
 * <p>This structure is used to describe input and output tensors.
 * Application can get input and output tensor type and shape described in model by using
 * {@link nnfw_input_tensorinfo} and {@link nnfw_output_tensorinfo}
 *
 * <p>Maximum rank is 6 (NNFW_MAX_RANK). And tensor's dimension value is filled in 'dims' field from
 * index 0.
 * For example, if tensor's rank is 4,
 * application can get dimension value from dims[0], dims[1], dims[2], and dims[3]
 */
typedef struct nnfw_tensorinfo
{
  /** The data type */
  NNFW_TYPE dtype;
  /** The number of dimensions (rank) */
  int32_t rank;
  /**
   * The dimension of tensor.
   * Maximum rank is 6 (NNFW_MAX_RANK).
   */
  int32_t dims[NNFW_MAX_RANK];
} nnfw_tensorinfo;

//////////////////////////////////////////////
// Essential APIs for training
//////////////////////////////////////////////
typedef enum
{
  NNFW_TRAIN_LOSS_UNDEFINED = 0,
  NNFW_TRAIN_LOSS_MEAN_SQUARED_ERROR = 1,
  NNFW_TRAIN_LOSS_CATEGORICAL_CROSSENTROPY = 2,
} NNFW_TRAIN_LOSS;

typedef enum
{
  /** Undefined */
  NNFW_TRAIN_LOSS_REDUCTION_UNDEFINED = 0,
  /** Scalar sum divided by number of elements in losses */
  NNFW_TRAIN_LOSS_REDUCTION_SUM_OVER_BATCH_SIZE = 1,
  /** Scalar sum of weighted losses */
  NNFW_TRAIN_LOSS_REDUCTION_SUM = 2,
} NNFW_TRAIN_LOSS_REDUCTION;

typedef enum
{
  NNFW_TRAIN_OPTIMIZER_UNDEFINED = 0,
  NNFW_TRAIN_OPTIMIZER_SGD = 1,
  NNFW_TRAIN_OPTIMIZER_ADAM = 2,
} NNFW_TRAIN_OPTIMIZER;

typedef struct nnfw_loss_info
{
  NNFW_TRAIN_LOSS loss;
  NNFW_TRAIN_LOSS_REDUCTION reduction_type;
} nnfw_loss_info;

typedef struct nnfw_adam_option
{
  float beta;
  float beta2;
  float epsilon;
} nnfw_adam_option;

/**
 * @brief Maximum numer of trainable operations
 */
#define NNFW_TRAINABLE_OPS_MAX_SIZE (256)

/**
 * @brief Training information to prepare training
 * @todo  Add more training information
 *        (e.g. optimizer, loss function, ...)
 */
typedef struct nnfw_train_info
{
  /** Learning rate */
  float learning_rate = 0.001f;
  /** Batch size */
  uint32_t batch_size = 1;
  /** loss info */
  nnfw_loss_info loss_info{.loss = NNFW_TRAIN_LOSS_CATEGORICAL_CROSSENTROPY,
                           .reduction_type = NNFW_TRAIN_LOSS_REDUCTION_SUM_OVER_BATCH_SIZE};
  /** optimizer type */
  NNFW_TRAIN_OPTIMIZER opt = NNFW_TRAIN_OPTIMIZER_ADAM;

  uint32_t num_trainble_ops = 0;

  nnfw_adam_option adam_opt{.beta = 0.9f,

                            .beta2 = 0.999f,
                            .epsilon = 1e-7f};
} nnfw_train_info;

/**
 * @brief Set training information
 * @note  This function should be called after calling {@link nnfw_load_model_from_file}
 *        and before calling {@link nnfw_train_prepare}
 *
 * @param[in] session The session to be set training information
 * @param[in] info    The training information
 *
 *  @return @c NNFW_STATUS_NO_ERROR if successful
 */
NNFW_STATUS nnfw_train_set_traininfo(nnfw_session *session, const nnfw_train_info *info);

/**
 * @brief Create a new session instance.
 *
 * <p>This only creates a session.
 * Model is loaded after {@link nnfw_load_model_from_file} is invoked.
 * And inference is performed after {@link nnfw_run} is invoked.
 *
 * <p>{@link nnfw_close_session} should be called once
 * if session is no longer needed
 *
 * @param[out]  session The session to be created
 * @return      NNFW_STATUS_NO_ERROR if successful
 */
NNFW_STATUS nnfw_create_session(nnfw_session **session);

/**
 * @brief Close a session instance
 *
 * After called, access to closed session by application will be invalid
 *
 * @param[in] session The session to be closed
 * @return    @c NNFW_STATUS_NO_ERROR if successful
 */
NNFW_STATUS nnfw_close_session(nnfw_session *session);

/**
 * @brief     Load model from nnpackage file or directory
 *
 * The length of \p package_file_path must not exceed 1024 bytes including zero at the end.
 *
 * @param[in] session           nnfw_session loading the given nnpackage file/dir
 * @param[in] package_file_path Path to the nnpackage file or unzipped directory to be loaded
 *
 * @return    @c NNFW_STATUS_NO_ERROR if successful
 */
NNFW_STATUS nnfw_load_model_from_file(nnfw_session *session, const char *package_file_path);

/**
 * @brief Prepare session to be ready for training
 * @note  The session will be entered into training mode
 *
 *        If training info is NOT set in session, this function returns @c NNFW_STATUS_ERROR .
 *        You should set training info using {@link nnfw_train_set_traininfo}.
 *
 * @param[in] session The session to be prepared for training
 *
 * @return  @c NNFW_STATUS_NO_ERROR if successful
 */
NNFW_STATUS nnfw_train_prepare(nnfw_session *session);

/**
 * @brief Train the model
 * @note  This function should be called after {@link nnfw_train_set_input} and
 *        {@link nnfw_train_set_expected} for each input and expected output
 *
 *        In order to use \p update_weights as false, it should be called after
 *        {@link nnfw_train_set_output}.
 *
 * @param[in] session The session to be trained
 * @param[in] update_weights If true, update weights of the model
 *                           If false, do not update weights of the model (for validation)
 * @return  @c NNFW_STATUS_NO_ERROR if successful
 */
NNFW_STATUS nnfw_train(nnfw_session *session, bool update_weights);

/**
 * @brief Export current training model into circle model
 * @note  This function should be called on training mode
 *        This function should be called after {@link nnfw_train}
 *
 * @param[in] session The session to export inference model
 * @param[in] path    The path to export inference model
 * @return @c NNFW_STATUS_NO_ERROR if successful
 */
NNFW_STATUS nnfw_train_export_circle(nnfw_session *session, const char *path);

NNFW_STATUS nnfw_train_export_checkpoint(nnfw_session *session, const char *path);
NNFW_STATUS nnfw_train_import_checkpoint(nnfw_session *session, const char *path);

/**
 * @brief Set training input
 * @note  This function should be called after {@link nnfw_train_prepare}
 *
 * @param[in] session     The session to be set training inputs and expected model outputs
 * @param[in] index       The index of training input
 * @param[in] input       The input buffers for training
 * @param[in] input_info  The shape and type of input buffer
 *                        If it is nullptr, it will not change shape and batch size
 * @return  @c NNFW_STATUS_NO_ERROR if successful
 */
NNFW_STATUS nnfw_train_set_input(nnfw_session *session, uint32_t index, void *input,
                                 const nnfw_tensorinfo *input_info);

/**
 * @brief Set training expected output
 * @note  This function should be called after {@link nnfw_train_prepare}
 *
 * @param session       The session to be set training inputs and expected model outputs
 * @param index         The index of training expected output
 * @param expected      The expected buffers for training
 * @param expected_info The shape and type of expected buffer
 *                      If it is nullptr, it will not change shape and batch size
 * @return  @c NNFW_STATUS_NO_ERROR if successful
 */
NNFW_STATUS nnfw_train_set_expected(nnfw_session *session, uint32_t index, void *expected,
                                    const nnfw_tensorinfo *expected_info);

/**
 * @brief Get loss value for expected output
 * @note  This function should be called after {@link nnfw_train}
 *
 * @param[in]   session The session to get loss value
 * @param[in]   index   The index of loss value [0, number of expected outputs)
 * @param[out]  loss    The loss value
 * @return  @c NNFW_STATUS_NO_ERROR if successful
 */
NNFW_STATUS nnfw_train_get_loss(nnfw_session *session, uint32_t index, float *loss);

/**
 * @brief Set training output buffer
 *
 * This function must be called after {@link nnfw_train_prepare}, \p buffer given to this function
 * can be reused for training. \p length must be greater or equal than the operand requires.
 * An output operand can have unspecified shape and deduced dynamically during the execution. You
 * must provide \p buffer large enough.
 *
 * @param[in]   session Session from inference output is to be extracted
 * @param[in]   index   Index of output to be set (0-indexed)
 * @param[in]   type    Type of the output
 * @param[out]  buffer  Raw buffer for output
 * @param[in]   length  Size of bytes of output buffer
 *
 * @return      @c NNFW_STATUS_NO_ERROR if successful
 */
NNFW_STATUS nnfw_train_set_output(nnfw_session *session, uint32_t index, NNFW_TYPE type,
                                  void *buffer, size_t length);

#ifdef __cplusplus
}
#endif

#endif //_ONERT_MICRO_H_
