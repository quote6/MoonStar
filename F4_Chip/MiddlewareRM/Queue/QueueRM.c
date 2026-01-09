/**
 * ************************************************************************
 * 
 * @file QueueRM.c
 * @author quote6 (2293127401@qq.com)
 * @brief 
 * @date 2025-6-15
 * @note 2025-6-13: 修复了 Queue_RM_TakeArrayOut 函数中的队列头指针更新错误bug \n
 *                  针对 QUEUE_RM_PARAMETER_CHECK 参数实现了参数检查功能
 * @note 2025-6-15：考虑了队列出队列操作与入队列操作同时运行时的安全性，并进行了修复。
 * @attention 对于队列可以同时进行入队列与出队列操作。但多个函数要同时进行 \n
 *            入队列和出队列操作时，要注意临界资源的保护                 
 * ************************************************************************
 * @copyright Copyright (c) 2025 quote6 
 * For study and research only, no reprinting
 * ************************************************************************
 */
#include "QueueRM.h"
#include <string.h>

/**
 * @brief 自定义内存拷贝函数
 * @param[in] destination  目标地址
 * @param[in] source  源地址
 * @param[in] size  数据大小
 * @note  定义该函数用于实现对任意数据大小的赋值工作
 * @attention 数据的大小不要超过uint32_t范围
 * @attention 注意输入参数的正确性，指针不能为空！！！
 */
inline static void RM_MemoryCopy(uint8_t* const destination, const uint8_t* const source, uint32_t const size) {
    #if 0
    for(uint32_t i = 0; i < size; i++){
        destination[i] = source[i];
    }
    #endif
    memcpy(destination, source, size);
}

// Queue_RM_t* Queue_RM_Create(void){
// }

// void Queue_RM_Free(Queue_RM_t* queue){
// }

/**
 * @brief 队列初始化代码
 * @param[in] queue  队列指针
 * @param[in] queueWidth  队列宽度, 缓冲区所占的 bit 宽度
 * @param[in] buff  缓冲区指针,设置为 uint8_t* 类型指针方便后续按字节对队列进行操作
 * @return uint8_t 函数执行状态
 *         @arg QUEUE_RM_STATUS_ERROR: 初始化失败，函数输入参数错误
 *         @arg QUEUE_RM_STATUS_SUCCESS: 初始化成功
 * @note  队列最大长度 = 1 << queueWidth
 * @attention  传入的数组要与 queueWidth 相匹配, 至少要避免指针越界的情况
 */
uint8_t QueueRM_Init(QueueRM_t* const queue, uint8_t const queueWidth, uint8_t* buff, uint8_t const elementSize) {
    /* 函数参数检查 */
    //如果队列和缓冲区指针为NULL，则判断函数输入参数错误，返回错误状态
    if(queue == NULL || buff == NULL){
        return QUEUE_RM_STATUS_ERROR;
    }
    /* 初始化操作 */
    queue->mask = (1 << queueWidth) - 1;//掩码值等于 队列最大长度 - 1
    queue->head = 0;
    queue->tail = 0;
    queue->buff = buff;
    queue->elementSize = elementSize;
    return QUEUE_RM_STATUS_SUCCESS;
}

/**
 * @brief 队列默认初始化函数
 * @param[in] queue  队列类型指针
 * @return uint8_t 函数执行状态
 *         @arg QUEUE_RM_STATUS_ERROR: 默认初始化失败，函数输入参数错误
 *         @arg QUEUE_RM_STATUS_SUCCESS: 默认初始化成功
 * @note  默认初始化后，队列的队头指针和队尾指针相等，且都为 0
 */
uint8_t QueueRM_DeInit(QueueRM_t* const queue) {
    /* 函数参数检查 */
    //如果队列为 NULL，则判断函数输入参数错误，返回错误状态
    if(queue == NULL){
        return QUEUE_RM_STATUS_ERROR;
    }
    // 重置头指针和尾指针 初始化队列长度为0
    queue->mask = 0;
    queue->head = 0;
    queue->tail = 0;
    queue->buff = NULL;
    queue->elementSize = 0;
    return QUEUE_RM_STATUS_SUCCESS;
}

/**
 * @brief 队列清空函数
 * @param[in] queue  队列指针
 * @return uint8_t 函数执行状态
 *         @arg QUEUE_RM_STATUS_ERROR: 执行失败，函数输入参数错误
 *         @arg QUEUE_RM_STATUS_SUCCESS: 执行成功
 * @note  将队列头指针和尾指针设置为 0, 表示清空队列中所有数据
 */
uint8_t QueueRM_Clear(QueueRM_t* const queue){
    /* 函数参数检查 */
    //如果队列为 NULL，则判断函数输入参数错误，返回错误状态
    if(queue == NULL){
        return QUEUE_RM_STATUS_ERROR;
    }
    queue->head = 0;
    queue->tail = 0;
    return QUEUE_RM_STATUS_SUCCESS;
}


/**
 * @brief 获取队列当前实际长度函数
 * @param[in] queue  队列类型指针
 * @return uint16_t  队列长度
 * @note  队列的长度 = (队尾指针 - 队头指针) % 队列最大长度
 * @note  因为获取队列长度较为简单且调用频繁，所以使用 inline 关键字修饰
 * @attention 注意队列指针不能为 NULL ！！！
 */
inline uint16_t QueueRM_GetLength(QueueRM_t* const queue) {
    //mask + 1代表队列缓冲区所占的内存长度
    return (queue->mask + 1 + queue->tail - queue->head) & queue->mask;
}

/**
 * @brief 向队列中插入元素
 * @param[in] queue  队列类型指针
 * @param[in] data  待插入的数据指针
 * @return uint8_t 函数执行状态
 *      @arg QUEUE_RM_STATUS_FULL: 队列满，本次插入元素失败
 *      @arg QUEUE_RM_STATUS_SUCCESS: 元素插入成功
 *      @arg QUEUE_RM_STATUS_ERROR: 执行失败，函数输入参数错误
 * @note  队列元素的插入过程: 先将 data 存入 tail 指向的数组位置，然后 tail 自增 1
 * @note  如果队列已满，则本次不会插入数据，即插入失败
 */
uint8_t QueueRM_PutIn(QueueRM_t* const queue,const void* const data) {
    #ifdef QUEUE_RM_PARAMETER_CHECK
    /* 函数参数检查 */
    //如果队列和待插入数据为 NULL，则判断函数输入参数错误，返回错误状态
    if(queue == NULL || data == NULL){
        return QUEUE_RM_STATUS_ERROR;
    }
    #endif

    // 当队列满后，返回队列满标志
    //  if((queue->tail + 1) % QUEUE_RM_MAX_LENGTH == queue->head) return QUEUE_RM_STATUS_FULL;
    if (((queue->tail + 1) & queue->mask) == queue->head)
        return QUEUE_RM_STATUS_FULL;
    else {
        // queue->buff[queue->tail++] = data;//【这样的赋值方式用于类比下面的赋值方式】
        RM_MemoryCopy((uint8_t*) queue->buff + queue->elementSize * queue->tail++,
                      (uint8_t*) data,
                      queue->elementSize);
        // queue->tail %= QUEUE_RM_MAX_LENGTH;
        queue->tail &= queue->mask;
    }
    return QUEUE_RM_STATUS_SUCCESS; // 返回插入成功状态
}

/**
 * @brief 从队列中取出元素
 * @param[in] queue  队列类型指针
 * @param[in] data  取出的数据将保存至该指针指向的位置
 * @return uint8_t 函数的执行状态
 *      @arg QUEUE_RM_STATUS_EMPTY: 队列为空，取元素失败
 *      @arg QUEUE_RM_STATUS_SUCCESS: 取元素成功
 *      @arg QUEUE_RM_STATUS_ERROR: 执行失败，函数输入参数错误
 * @note  从队列中取元素过程，先将 head 指向数组位置的数据 \n
 *        存到 data 指针指向的地址处，然后 head 自增 1
 */
uint8_t QueueRM_TakeOut(QueueRM_t* const queue, void* const data) {
    #ifdef QUEUE_RM_PARAMETER_CHECK
    /* 函数参数检查 */
    //如果队列和待插入数据为 NULL，则判断函数输入参数错误，返回错误状态
    if(queue == NULL || data == NULL){
        return QUEUE_RM_STATUS_ERROR;
    }
    #endif

    if (queue->head == queue->tail)
        return QUEUE_RM_STATUS_EMPTY; // 返回队列空状态
    else {
        // *data = queue->buff[queue->head++];//【这样的赋值方式用于类比下面的赋值方式】
        RM_MemoryCopy((uint8_t*) data,
                      (uint8_t*) queue->buff + queue->elementSize * queue->head++,
                      queue->elementSize);
        // queue->head %= QUEUE_RM_MAX_LENGTH;
        queue->head &= queue->mask;
        return QUEUE_RM_STATUS_SUCCESS; // 返回取元素成功状态
    }
}

/**
 * @brief 向队列中存入一个数组
 * @param[in] queue  队列类型指针
 * @param[in] source  数据源数组指针
 * @param[in] lengthTarget  指定存入的长度
 * @param[in] lengthActual  实际存入的长度
 * @return uint8_t 函数的执行状态
 *      @arg QUEUE_RM_STATUS_INCOMPLETE: 并未完成整个存入过程，可能没有存入数据或者只存入部分数据
 *      @arg QUEUE_RM_STATUS_SUCCESS: 完成整个存入过程
 *      @arg QUEUE_RM_STATUS_ERROR: 执行失败，函数输入参数错误
 * @note  当数组存入的长度大于队列的剩余长度时，数组的数据会发生截断， \n
 *        只会将数组的前半部分数据存入队列,并使队列为数据满状态。
 * @todo 待检验
 */
uint8_t QueueRM_PutArrayIn(QueueRM_t* const queue,const void* const source, uint16_t lengthTarget, uint16_t* const lengthActual) {
    #ifdef QUEUE_RM_PARAMETER_CHECK
    /* 函数参数检查 */
    //如果队列和待插入数据源为 NULL，则判断函数输入参数错误，返回错误状态
    if(queue == NULL || source == NULL){
        return QUEUE_RM_STATUS_ERROR;
    }
    #endif
    
    /* 暂存出队列指针（因为队列可以同时进行入队和出队，但这里的函数内部要确保头指针是同一个值，不能发生变化，否则影响执行结果） */
    uint16_t head = queue->head;
    /* 暂存queue的位置（不过一般队列正常情况下，同一时间里只能有一个函数进行入队操作，所以在这个函数内部尾指针都是同一个值，所以不用暂存） */
    // uint16_t tail = queue->tail;

    uint8_t result = QUEUE_RM_STATUS_INCOMPLETE;
    
    /* 计算队列的剩余长度 */
    uint16_t lengthRemain = (queue->mask + head - queue->tail) & queue->mask;

    /* 数组末端剩余空间
       末端剩余空间表示的是 在完成插入后队列尾指针变为0的插入长度，
       因为结果可能超过 uint16_t 范围，所以使用 uint32_t 类型变量
    */
    uint32_t lengthEndRemain = queue->mask - queue->tail + 1;//
    
    /* 存储实际插入长度值 */
    uint16_t length;

    // 判断剩余空间是否可以将整个数组完全存入 如果不能完全存入会发生数据的截断，即只存入数组的前半部分数据
    if (lengthTarget > lengthRemain) {
        // *lengthActual = lengthTarget;
        length = lengthRemain;
    } else {
        result = QUEUE_RM_STATUS_SUCCESS;
        // *lengthActual = lengthRemain;
        length = lengthTarget;
    }
    // 如果 lengthActual 不为空，则将实际插入个数赋值给它
    if (lengthActual != NULL)
        *lengthActual = length;

    /* =========简洁写法(begin)==================== */
    // 将数组中的数据插入队列
    // for (uint16_t i = 0; i < length; i++) {
    //     // queue->buff[queue->tail++] = source[i];
    //     RM_MemoryCopy((uint8_t*) queue->buff + queue->elementSize * queue->tail++, (uint8_t*) source + queue->elementSize * i, queue->elementSize);
    //     // queue->tail %= QUEUE_RM_MAX_LENGTH;
    //     queue->tail &= queue->mask;
    // }
    /* =========简洁写法(end)====================== */


    /* =========高效写法(begin)==================== */
    //减少对指针的 & 运算
    //【前提】这里的写入操作并不会使队列溢出，不会溢出由 length 的计算方式保证
    //(1)队尾指针小于队头指针，则可以一次性写入所有数据
    //(2)在队尾大于队头指针的情况下，如果要插入的长度小于数组末端剩余空间，
    //   也就是插入完成后队头指针不会循环，那么一次性写入所有数据
    if(length < lengthEndRemain){
    // if((queue->tail < head) || (length < lengthEndRemain) ){
        // for(uint16_t i = 0; i < length; i++){
        //     // queue->buff[queue->tail + i] = source[i];//【这样的赋值方式用于类比下面的赋值方式】
        //     RM_MemoryCopy((uint8_t*) queue->buff + queue->elementSize * (queue->tail + i),
        //                   (uint8_t*) source + queue->elementSize * i,
        //                   queue->elementSize);
        // }
        RM_MemoryCopy((uint8_t*) queue->buff + queue->elementSize * queue->tail,
                      (uint8_t*) source,
                      queue->elementSize * length);
        //更新队尾指针
        queue->tail += length;
    }
    //当不满足条件时，就需要分两次取出
    else {
        //第1次写完末尾所有空余位置

        // for(uint16_t i = 0; i < lengthEndRemain; i++){
        //     // queue->buff[queue->tail + i] = source[i];//【这样的赋值方式用于类比下面的赋值方式】
        //     RM_MemoryCopy((uint8_t*) queue->buff + queue->elementSize * (queue->tail + i),
        //                   (uint8_t*) source + queue->elementSize * i,
        //                   queue->elementSize);
        // }
        RM_MemoryCopy((uint8_t*) queue->buff + queue->elementSize * queue->tail,
                      (uint8_t*) source,
                      queue->elementSize * lengthEndRemain);

        //第2次在开头写入剩余数据

        // for(uint16_t i = 0; i < (length - lengthEndRemain); i++){
        //     // queue->buff[i] = source[i + lengthEndRemain];//【这样的赋值方式用于类比下面的赋值方式】
        //     RM_MemoryCopy((uint8_t*) queue->buff + queue->elementSize * i,
        //                   (uint8_t*) source + queue->elementSize * (lengthEndRemain + i),
        //                   queue->elementSize);
        // }
        RM_MemoryCopy((uint8_t*) queue->buff,
                      (uint8_t*) source + queue->elementSize * lengthEndRemain,
                      queue->elementSize * (length - lengthEndRemain));
        
        //更新队尾指针
        queue->tail = (length - lengthEndRemain);
    }
    /* =========高效写法(end)====================== */

    return result;
}

/**
 * @brief 从队列中取出一组数据
 * @param[in] queue  队列类型指针
 * @param[in] destination  取出的数据将保存至该数组指针
 * @param[in] lengthTarget  要取出的数据长度
 * @param[in] lengthActual  实际取出的数据长度
 * @return uint8_t 函数的执行状态
 *      @arg QUEUE_RM_STATUS_INCOMPLETE: 未完成整个取出过程，可能没有取出元素，或只取出部分元素
 *      @arg QUEUE_RM_STATUS_SUCCESS: 完成整个取出过程
 *      @arg QUEUE_RM_STATUS_ERROR: 执行失败，函数输入参数错误
 * @note  如果队列的当前长度不够要取出的数组长度，则将当前队列的全部数据都取出，存入数组中
 */
uint8_t QueueRM_TakeArrayOut(QueueRM_t* const queue, void* const destination, uint16_t lengthTarget, uint16_t* const lengthActual) {
    
    #ifdef QUEUE_RM_PARAMETER_CHECK
    /* 函数参数检查 */
    //如果队列和目的数组为 NULL，则判断函数输入参数错误，返回错误状态
    if(queue == NULL || destination == NULL){
        return QUEUE_RM_STATUS_ERROR;
    }
    #endif
    /* 暂存入队列指针值 （因为队列可以同时进行入队和出队，但这里的函数内部要确保尾指针是同一个值，不能发生变化，否则影响执行结果）*/
    uint16_t tail = queue->tail;

    uint8_t result = QUEUE_RM_STATUS_INCOMPLETE;
    
    /* 队列当前长度 */
    uint16_t lengthCurrent = (queue->mask + 1 + tail - queue->head) & queue->mask;

    // 末端剩余空间表示的是 在完成出队列后队列头指针变为0的出队列长度 
    // 因为结果可能超过 uint16_t 范围，所以使用 uint32_t 类型变量
    uint32_t lengthEndRemain = queue->mask - queue->head + 1;//数组末端剩余空间
    uint16_t length;

    // 判断当前队列数据长度是否可以将整个数组完全填充 如果不足，则将队列中当前所有的数据取出
    if (lengthTarget > lengthCurrent) {
        length = lengthCurrent;
    } else {
        result = QUEUE_RM_STATUS_SUCCESS;
        length = lengthTarget;
    }
    // 如果 lengthActual 不为空，则将实际取出的个数赋值给它
    if(lengthActual != NULL)
        *lengthActual = length;

    /* =========简洁写法(begin)==================== */
    // // 将队列中的数据取出来赋值给数组
    // for (uint16_t i = 0; i < length; i++) {
    //     // destination[i] = queue->buff[queue->head++];
    //     RM_MemoryCopy((uint8_t*) destination,
    //                   (uint8_t*) queue->buff + queue->elementSize * queue->head++,
    //                   queue->elementSize);
    //     // queue->head %= QUEUE_RM_MAX_LENGTH;
    //     queue->head &= queue->mask;
    // }
    /* =========简洁写法(end)====================== */

    
    /* =========高效写法(begin)==================== */
    //减少对指针的 & 运算
    //【前提】这里的写入操作并不会使队列溢出，不会溢出由 length 的计算方式保证
    //(1)如果队头指针小于队尾指针，则可一次性取出数据
    //(2)如果队列数组的末端剩余空间大于等于要取的数据长度
    //也就是队头指针不会发生循环，则可一次性取出数据
    if(length < lengthEndRemain){
    // if((queue->head < tail)  || (length < lengthEndRemain)){
        // for(uint16_t i = 0; i < length; i++){
        //     // destination[i] = queue->buff[queue->head + i];
        //     RM_MemoryCopy((uint8_t*) destination + queue->elementSize * i,
        //                   (uint8_t*) queue->buff + queue->elementSize * (queue->head + i),
        //                   queue->elementSize);
        // }
        RM_MemoryCopy((uint8_t*) destination,
                      (uint8_t*) queue->buff + queue->elementSize * queue->head,
                      queue->elementSize * length);
        //更新队头指针
        queue->head += length;
    }
    //当不满足条件时就需要分两次取出
    else{
        //先取出末尾所有的数据

        // for(uint16_t i = 0; i < lengthEndRemain; i++){
        //     // destination[i] = queue->buff[queue->head + i];
        //     RM_MemoryCopy((uint8_t*) destination + queue->elementSize * i,
        //                   (uint8_t*) queue->buff + queue->elementSize * (queue->head + i),
        //                   queue->elementSize);
        // }
        RM_MemoryCopy((uint8_t*) destination,
                      (uint8_t*) queue->buff + queue->elementSize * queue->head,
                      queue->elementSize * lengthEndRemain);

        //再取出开头的剩余数据

        // for(uint16_t i = 0; i < (length - lengthEndRemain); i++){
        //     // destination[lengthEndRemain + i] = queue->buff[i];
        //     RM_MemoryCopy((uint8_t*) destination + queue->elementSize * (lengthEndRemain + i),
        //                   (uint8_t*) queue->buff + queue->elementSize * i,
        //                   queue->elementSize);
        // }
        RM_MemoryCopy((uint8_t*) destination + queue->elementSize * lengthEndRemain,
                      (uint8_t*) queue->buff,
                      queue->elementSize * (length - lengthEndRemain));
        //更新队头指针
        queue->head = (length - lengthEndRemain);
    }
    /* =========高效写法(end)====================== */

    return result;
}

/**
 * @brief 将队列的数据搬运到另一个队列
 * @param[in] queueD  目标队列
 * @param[in] queueS  源队列
 * @param[in] lengthTarget  要搬运的数据长度
 * @param[in] lengthActual  实际搬运的数据长度
 * @return uint8_t 函数的执行状态
 *      @arg QUEUE_RM_STATUS_INCOMPLETE: 未完成整个取出过程，可能没有取出元素，或只取出部分元素
 *      @arg QUEUE_RM_STATUS_SUCCESS: 完成整个取出过程
 *      @arg QUEUE_RM_STATUS_ERROR: 执行失败，函数输入参数错误
 * @note  如果队列的当前长度不够要取出的数据度，则将当前队列的全部数据都取出，存入目标队列中
 *        如果目标队列的剩余长度不足以存放数据，则会发生数据截断
 * @attention 必须保证进行搬运的两个队列的单个元素大小一致！！！
 */
uint8_t QueueRM_TransferData(QueueRM_t* const queueD, QueueRM_t* const queueS, uint16_t lengthTarget, uint16_t* const lengthActual){
    #ifdef QUEUE_RM_PARAMETER_CHECK
    /* 函数参数检查 */
    //如果队列和目的数组为 NULL，或者两个对了的元素大小不一致，则判断函数输入参数错误，返回错误状态
    if (queueS == NULL || queueD == NULL || (queueD->elementSize != queueS->elementSize)) {
        return QUEUE_RM_STATUS_ERROR;
    }
    #endif
    uint8_t result = QUEUE_RM_STATUS_INCOMPLETE;
    /* 实际搬运长度 */
    uint16_t length;

    /* 暂存源队列入队列指针值 （因为队列可以同时进行入队和出队，但这里的函数内部要确保尾指针是同一个值，不能发生变化，否则影响执行结果）*/
    uint16_t tailS = queueS->tail;
    /* 存储源队列的实际可以取出的长度值 */
    uint16_t lengthS;
    /* 计算源队列当前长度 */
    uint16_t lengthCurrentS = (queueS->mask + 1 + tailS - queueS->head) & queueS->mask;
    /* 
        计算原队列数组末端剩余空间 
        末端剩余空间表示的是 在完成出队列后队列头指针变为0的出队列长度 
        因为结果可能超过 uint16_t 范围，所以使用 uint32_t 类型变量
    */
    uint32_t lengthEndRemainS = queueS->mask - queueS->head + 1;//数组末端剩余空间
    // 判断当前队列数据长度是否可以取出目标个数数据 如果不足，则将队列中当前所有的数据取出
    if (lengthTarget > lengthCurrentS) {
        lengthS = lengthCurrentS;
    } else {
        lengthS = lengthTarget;
    }

    /* 暂存目标队列出队列指针（因为队列可以同时进行入队和出队，但这里的函数内部要确保头指针是同一个值，不能发生变化，否则影响执行结果） */
    uint16_t headD = queueD->head;
    /* 存储目标队列的实际可以插入的长度值 */
    uint16_t lengthD;
    /* 计算目标队列的剩余长度 */
    uint16_t lengthRemainD = (queueD->mask + headD - queueD->tail) & queueD->mask;
    /* 
        计算目标数组末端剩余空间
        末端剩余空间表示的是 在完成插入后队列尾指针变为0的插入长度，
        因为结果可能超过 uint16_t 范围，所以使用 uint32_t 类型变量
    */
    uint32_t lengthEndRemainD = queueD->mask - queueD->tail + 1;
    // 判断剩余空间是否可以将目标个数数据完全存入 如果不能完全存入会发生数据的截断，即只存入数组的前半部分数据
    if (lengthTarget > lengthRemainD) {
        lengthD = lengthRemainD;
    } else {
        lengthD = lengthTarget;
    }
    /* 计算实际插入长度（取源队列和目标队列的可操作数据长度的最小值来执行） */
    length = (lengthS < lengthD)? lengthS : lengthD;
    /* 如果可操作长度满足参数指定的长度则返回成功状态 */
    if(length == lengthTarget){
        result = QUEUE_RM_STATUS_SUCCESS;
    }
    /* 如果 lengthActual 不为空，则将实际搬运的个数赋值给它 */
    if (lengthActual != NULL){
        *lengthActual = length;
    }
    /* 开始数据搬运 */
    /* =========简洁写法(begin)==================== */
    // /* 将队列中的数据取出来赋值给数组 */ 
    // for (uint16_t i = 0; i < length; i++) {
    //     // destination[i] = queue->buff[queue->head++];
    //     RM_MemoryCopy(
    //         (uint8_t*) queueD->buff + queueD->elementSize * queueD->tail++,
    //         (uint8_t*) queueS->buff + queueS->elementSize * queueS->head++,
    //         queueS->elementSize
    //     );
    //     /* 更新目标队列的入队列指针 */
    //     // queueD->tail %= QUEUE_RM_MAX_LENGTH;
    //     queueD->tail &= queueD->mask;
    //     /* 更新源队列的出队列指针 */
    //     // queueS->head %= QUEUE_RM_MAX_LENGTH;
    //     queueS->head &= queueS->mask;
    // }
    /* =========简洁写法(end)====================== */

    /* =========高效写法(begin)==================== */
    //减少对指针的 & 运算
    //【前提】这里的写入操作并不会使队列溢出，不会溢出由 length 的计算方式保证
    /* 
        源队列情况
            如果队列数组的末端剩余空间大于等于要取的数据长度
            也就是队头指针不会发生循环，则可一次性取出数据
        目标队列情况
            如果要插入的长度小于数组末端剩余空间，
            也就是插入完成后队头指针不会循环，那么一次性写入所有数据
    */
    /* 满足条件可以从源队列一次性取数据 */
    if(length < lengthEndRemainS){
        /* 满足条件可以往目标队列一次性存数据 */
        if (length < lengthEndRemainD) {
            RM_MemoryCopy(
                (uint8_t*) queueD->buff + queueD->elementSize * queueD->tail,
                (uint8_t*) queueS->buff + queueS->elementSize * queueS->head,
                queueD->elementSize * length
            );
            /* 更新目标队列尾指针 */
            queueD->tail += length;
        }
        /* 否则需要分两次往目标队列存数据 */
        else {
            /* 第1次将目标队列的末端剩余空间全部写入 */
            RM_MemoryCopy(
                (uint8_t*) queueD->buff + queueD->elementSize * queueD->tail,
                (uint8_t*) queueS->buff + queueS->elementSize * queueS->head,
                queueD->elementSize * lengthEndRemainD
            );
            /* 第2次在目标队列的开头写入剩余数据 */
            RM_MemoryCopy(
                (uint8_t*) queueD->buff,
                (uint8_t*) queueS->buff + queueS->elementSize * queueS->head + queueD->elementSize * lengthEndRemainD,
                queueD->elementSize *(length - lengthEndRemainD)
            );
            /* 更新目标队列尾指针 */
            queueD->tail = (length - lengthEndRemainD);
        }
        /* 更新源队列头指针 */
        queueS->head += length;
    }
    /* 不满足条件需要分两次从源队列取数据 */
    else{
        /* 满足条件可以往目标队列一次性存数据 */
        if (length < lengthEndRemainD) {
            /* 第1次将源队列的末端剩余空间数据全部取出 */
            RM_MemoryCopy(
                (uint8_t*) queueD->buff + queueD->elementSize * queueD->tail,
                (uint8_t*) queueS->buff + queueS->elementSize * queueS->head,
                queueS->elementSize * lengthEndRemainS
            );
            /* 第2次将源队列开头剩余数据全部取出 */
            RM_MemoryCopy(
                (uint8_t*) queueD->buff + queueD->elementSize * queueD->tail + queueS->elementSize * lengthEndRemainS,
                (uint8_t*) queueS->buff,
                queueS->elementSize * (length - lengthEndRemainS)
            );
            /* 更新目标队列指针 */
            queueD->tail += length;
        }
        /* 否则需要分两次往目标队列存数据 */
        else{
            /* 以下语句判断需要进行几次搬运（最少2次，最多3次，这里为了方便直接以3次进行书写） */
            if(lengthEndRemainS > lengthEndRemainD){
                RM_MemoryCopy(
                    (uint8_t*) queueD->buff + queueD->elementSize * queueD->tail,
                    (uint8_t*) queueS->buff + queueS->elementSize * queueS->head,
                    queueD->elementSize * lengthEndRemainD
                );
                RM_MemoryCopy(
                    (uint8_t*) queueD->buff,
                    (uint8_t*) queueS->buff + queueS->elementSize * queueS->head + queueD->elementSize * lengthEndRemainD,
                    queueS->elementSize * (lengthEndRemainS - lengthEndRemainD)
                );
                RM_MemoryCopy(
                    (uint8_t*) queueD->buff + queueS->elementSize * (lengthEndRemainS - lengthEndRemainD),
                    (uint8_t*) queueS->buff,
                    queueS->elementSize * (length - lengthEndRemainS)
                );
            }
            else{
                RM_MemoryCopy(
                    (uint8_t*) queueD->buff + queueD->elementSize * queueD->tail,
                    (uint8_t*) queueS->buff + queueS->elementSize * queueS->head,
                    queueS->elementSize * lengthEndRemainS
                );
                RM_MemoryCopy(
                    (uint8_t*) queueD->buff + queueD->elementSize * queueD->tail + queueS->elementSize * lengthEndRemainS,
                    (uint8_t*) queueS->buff,
                    queueD->elementSize * (lengthEndRemainD - lengthEndRemainS)
                );
                RM_MemoryCopy(
                    (uint8_t*) queueD->buff,
                    (uint8_t*) queueS->buff + queueD->elementSize * (lengthEndRemainD - lengthEndRemainS),
                    queueD->elementSize * (length - lengthEndRemainD)
                );
            }
            /* 更新目标队列指针 */
            queueD->tail = (length - lengthEndRemainD);
        }
        /* 更新源队列头指针 */
        queueS->head = (length - lengthEndRemainS);
    }
    /* =========高效写法(end)====================== */
    return result;
}



/**
 * @brief 丢弃队列中部分元素
 * @param[in] queue  队列类型指针
 * @param[in] lengthTarget  丢弃的目标长度
 * @param[in] lengthActual  实际丢弃的长度
 * @return uint8_t 函数执行的状态
 *      @arg QUEUE_RM_STATUS_INCOMPLETE: 未完成整个丢弃过程，只丢弃了部分元素
 *      @arg QUEUE_RM_STATUS_SUCCESS: 完成整个丢弃过程
 *      @arg QUEUE_RM_STATUS_ERROR: 执行失败，函数输入参数错误
 * @note  丢弃的含义，直接丢弃从队列的队头指针开始的若干个元素，然后队头元素指针自增若干值
 */
uint8_t QueueRM_Discard(QueueRM_t* const queue, uint16_t lengthTarget,uint16_t* const lengthActual) {
    #ifdef QUEUE_RM_PARAMETER_CHECK
    /* 函数参数检查 */
    //如果队列为 NULL，则判断函数输入参数错误，返回错误状态
    if(queue == NULL){
        return QUEUE_RM_STATUS_ERROR;
    }
    #endif
    uint8_t result = QUEUE_RM_STATUS_INCOMPLETE;
    uint16_t lengthCurrent = QueueRM_GetLength(queue); // 当前队列数据长度
    uint16_t length;
    // 判断当前队列数据长度是否可以将丢弃目标个数的数量 如果不足，则将队列中所有的数据丢弃
    if (lengthTarget > lengthCurrent) {
        length = lengthCurrent;
    } else {
        result = QUEUE_RM_STATUS_SUCCESS;
        length = lengthTarget;
    }
    // 如果 lengthActual 不为空，则将实际丢弃的个数赋值给它
    if(lengthActual != NULL)
        *lengthActual = length;

    queue->head += length;
    queue->head &= queue->mask;

    return result;
}

/**
 * @brief 队列覆写函数
 * @param[in] queue  队列类型指针
 * @param[in] data  待插入的数据
 * @return uint8_t 函数的执行状态
 *      @arg QUEUE_RM_STATUS_SPARE: 队列有空余，本次插入没有丢弃队头元素
 *      @arg QUEUE_RM_STATUS_FULL: 队列已满，本次插入，队头的元素已被丢弃
 *      @arg QUEUE_RM_STATUS_ERROR: 执行失败，函数输入参数错误
 * @note  当队列处于满状态时，会发生覆写，丢弃队头的元素，然后在队尾插入数据
 * @attention 这函数本质上有入队与出队的操作，在调用时要注意临界资源的保护
 */
uint8_t QueueRM_Overwrite(QueueRM_t* const queue,const void* const data) {
    #ifdef QUEUE_RM_PARAMETER_CHECK
    /* 函数参数检查 */
    //如果队列和待插入数据为 NULL，则判断函数输入参数错误，返回错误状态
    if(queue == NULL || data == NULL){
        return QUEUE_RM_STATUS_ERROR;
    }
    #endif

    uint8_t result = QUEUE_RM_STATUS_SPARE; // 默认状态队列为空余状态
    // 如果队列已满
    if (((queue->tail + 1) & queue->mask) == queue->head) {
        // 将队头指针指向的元素丢弃，并且队头指针自增
        queue->head++;
        queue->head &= queue->mask;
        result = QUEUE_RM_STATUS_FULL; // 设置为满状态
    }

    // queue->buff[queue->tail++] = data;
    RM_MemoryCopy((uint8_t*) queue->buff + queue->elementSize * queue->tail++,
                  (uint8_t*) data,
                  queue->elementSize);
    queue->tail &= queue->mask;

    return result;
}

/**
 * @brief 查看队列的队头元素
 * @param[in] queue  队列类型指针
 * @param[in] data  查看得到的元素将存入该指针指向的地址
 * @return uint8_t 函数执行的状态
 *      @arg QUEUE_RM_STATUS_SUCCESS: 查看成功
 *      @arg QUEUE_RM_STATUS_FAILURE: 查看失败，因为队列为空
 *      @arg QUEUE_RM_STATUS_ERROR: 执行失败，函数输入参数错误
 * @note  查看队头元素，不会改变队头指针的值
 */
uint8_t QueueRM_ViewHead(const QueueRM_t* const queue, void* const data) {
    #ifdef QUEUE_RM_PARAMETER_CHECK
    /* 函数参数检查 */
    //如果队列和接收数据指针为 NULL，则判断函数输入参数错误，返回错误状态
    if(queue == NULL || data == NULL){
        return QUEUE_RM_STATUS_ERROR;
    }
    #endif
    // 当队列非空
    if (queue->head != queue->tail) {
        // *data = queue->buff[queue->head];
        RM_MemoryCopy((uint8_t*) data,
                      (uint8_t*) queue->buff + queue->elementSize * queue->head,
                      queue->elementSize);
        return QUEUE_RM_STATUS_SUCCESS;
    } else {
        return QUEUE_RM_STATUS_FAILURE;
    }
}

/**
 * @brief 查看队尾元素
 * @param[in] queue  队列类型指针
 * @param[in] data  查看得到的数据将存入该指针指向的地址
 * @return uint8_t 函数的执行状态
 *      @arg QUEUE_RM_STATUS_SUCCESS: 查看成功
 *      @arg QUEUE_RM_STATUS_FAILURE: 查看失败，因为队列为空
 *      @arg QUEUE_RM_STATUS_ERROR: 执行失败，函数输入参数错误
 * @note  查看队尾元素，不会改变队尾指针的值
 */
uint8_t QueueRM_ViewTail(const QueueRM_t* const queue, void* const data) {
    #ifdef QUEUE_RM_PARAMETER_CHECK
    /* 函数参数检查 */
    //如果队列和接收数据指针为 NULL，则判断函数输入参数错误，返回错误状态
    if(queue == NULL || data == NULL){
        return QUEUE_RM_STATUS_ERROR;
    }
    #endif

    /* 先暂存入队列指针 确保该函数内入队列指针都是同一个值 */
    uint16_t index = queue->tail;

    // 如果队列非空
    if (queue->head != index) {
        // 因为队尾指针指向的是当前要存储的位置，上一次存储的数据在上一个位置，所以需要对指针减一
        index--;
        index &= queue->mask;
        // *data = queue->buff[index];
        RM_MemoryCopy((uint8_t*) data,
                      (uint8_t*) queue->buff + queue->elementSize * index,
                      queue->elementSize);
        return QUEUE_RM_STATUS_SUCCESS;
    } else {
        return QUEUE_RM_STATUS_FAILURE;
    }
}

/**
 * @brief 偷看队列中的数据
 * @param[in] queue  队列类型指针
 * @param[in] destination  将偷看到的一系列数据存入该数组指向的位置中
 * @param[in] length  要偷看的数据长度
 * @param[in] lengthActual  实际偷看到的数据长度
 * @return uint8_t 函数的执行状态
 *      @arg QUEUE_RM_STATUS_INCOMPLETE: 并未完成整个偷看过程，只偷看到部分数据
 *      @arg QUEUE_RM_STATUS_SUCCESS: 完成整个数据偷看过程，偷看到 length 个数据
 *      @arg QUEUE_RM_STATUS_ERROR: 执行失败，函数输入参数错误
 * @note  偷看队列中的数据，并不会改变队头指针的值。
 * @note  若队列中的当前长度小于设定的偷看长度，则将当前队列中的所有数据拷贝到 destination 指针指向的位置处
 */
uint8_t QueueRM_Peep(const QueueRM_t* const queue, void* const destination, uint16_t lengthTarget, uint16_t* const lengthActual) {
    #ifdef QUEUE_RM_PARAMETER_CHECK
    /* 函数参数检查 */
    //如果队列和接收数据数组为 NULL，则判断函数输入参数错误，返回错误状态
    if(queue == NULL || destination == NULL){
        return QUEUE_RM_STATUS_ERROR;
    }
    #endif

    /* 暂存入队列指针值 （因为队列可以同时进行入队和出队，但这里的函数内部要确保尾指针是同一个值，不能发生变化，否则影响执行结果）*/
    uint16_t tail = queue->tail;

    uint8_t result = QUEUE_RM_STATUS_INCOMPLETE;

    /* 队列当前长度 */
    uint16_t lengthCurrent = (queue->mask + 1 + tail - queue->head) & queue->mask;

    // 末端剩余空间表示的是 在完成出队列后队列头指针变为0的出队列长度 
    // 因为结果可能超过 uint16_t 范围，所以使用 uint32_t 类型变量
    uint32_t lengthEndRemain = queue->mask - queue->head + 1;//数组末端剩余空间
    uint16_t length;
    
    // 判断当前队列数据长度是否可以将可以满足偷看的个数 如果不足，则将队列中当前所有的数据取出
    if (lengthTarget > lengthCurrent) {
        length = lengthCurrent;
    } else {
        result = QUEUE_RM_STATUS_SUCCESS;
        length = lengthTarget;
    }
    // 如果 lengthActual 不为空，则将实际偷看的个数赋值给它
    if(lengthActual != NULL)
        *lengthActual = length;


    /* =========简洁写法(begin)==================== */
    // uint16_t index = queue->head;
    // // 将队列中的数据取出来赋值给数组
    // for (uint16_t i = 0; i < lengthActual; i++) {
    //     // destination[i] = queue->buff[index++];
    //     RM_MemoryCopy((uint8_t*) destination + queue->elementSize * i,
    //                   (uint8_t*) queue->buff + queue->elementSize * index++,
    //                   queue->elementSize);
    //     index &= queue->mask;
    // }
    /* =========简洁写法(end)====================== */

    /* =========高效写法(begin)==================== */
    //减少对指针的 & 运算

    //(1)如果队头指针小于队尾指针，则可一次性取出数据
    //(2)如果队列数组的末端剩余空间大于要取的数据长度
    //也就是队头指针不会发生循环，则可一次性取出数据
    if(length < lengthEndRemain){
    // if((queue->head < tail)  || (length < lengthEndRemain)){
        // for(uint16_t i = 0; i < length; i++){
        //     // destination[i] = queue->buff[queue->head + i];
        //     RM_MemoryCopy((uint8_t*) destination + queue->elementSize * i,
        //                   (uint8_t*) queue->buff + queue->elementSize * (queue->head + i),
        //                   queue->elementSize);
        // }
        RM_MemoryCopy((uint8_t*) destination,
                      (uint8_t*) queue->buff + queue->elementSize * queue->head,
                      queue->elementSize * length);
    }
    //当不满足条件时就需要分两次取出
    else{
        //先取出末尾所有的数据

        // for(uint16_t i = 0; i < lengthEndRemain; i++){
        //     // destination[i] = queue->buff[queue->head + i];
        //     RM_MemoryCopy((uint8_t*) destination + queue->elementSize * i,
        //                   (uint8_t*) queue->buff + queue->elementSize * (queue->head + i),
        //                   queue->elementSize);
        // }
        RM_MemoryCopy((uint8_t*) destination,
                      (uint8_t*) queue->buff + queue->elementSize * queue->head,
                      queue->elementSize * lengthEndRemain);
        
        //再取出开头的剩余数据

        // for(uint16_t i = 0; i < (length - lengthEndRemain); i++){
        //     // destination[lengthEndRemain + i] = queue->buff[i];
        //     RM_MemoryCopy((uint8_t*) destination + queue->elementSize * (lengthEndRemain + i),
        //                   (uint8_t*) queue->buff + queue->elementSize * i,
        //                   queue->elementSize);
        // }
        RM_MemoryCopy((uint8_t*) destination + queue->elementSize * lengthEndRemain,
                      (uint8_t*) queue->buff,
                      queue->elementSize * (length - lengthEndRemain));
    }
    /* =========高效写法(end)====================== */
    return result;
}
