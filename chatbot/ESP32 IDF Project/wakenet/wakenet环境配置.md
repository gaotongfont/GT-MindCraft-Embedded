# WakeNet 环境依赖配置

## 1 环境下载

补丁存放在微盘 __Esp_WakeNet 环境补丁__ 文件夹中.

### 1.1 ADF-v2.7 安装

拉取官方adf仓库

```shell
git clone https://github.com/espressif/esp-adf.git esp-adf-v2.7

cd esp-adf-v2.7

git checkout v2.7

git submodule update --init --recursive
```

### 1.2 adf 补丁更新

1. 将 __gt_patches/adf/__ 文件夹中 _esp32_adf_v2_7.patch_ 复制到 __esp-adf-v2.7__ 目录下;

2. 在 __esp-adf-v2.7__ 目录下, 执行 `git apply --reject esp32_adf_v2_7.patch` 等待补丁执行;

    ```shell
    cd esp-adf-v2.7

    git apply --reject esp32_adf_v2_7.patch
    ```

3. 将 __gt_patches/adf/__ 文件夹中的 _adf_components_esp_sr.patch_ 复制到 __esp-adf-v2.7/components/esp-sr/__ 目录下;

4. 进入到 adf/components/esp-sr/ 目录下, 执行 `git apply --reject adf_components_esp_sr.patch`;

    ```shell
    cd ./components/esp-sr/

    git apply --reject adf_components_esp_sr.patch
    ```

5. 检查所有补丁正常被执行.

### 1.3 IDF-v5.4.0 安装

```shell
git clone -b v5.4 --recursive https://github.com/espressif/esp-idf.git esp-idf-v5.4

cd esp-idf-v5.4/
```

### 1.4 idf 补丁更新

1. 将 __gt_patches/idf/__ 文件夹的 _esp_idf_websocket_client.patch_ 复制到 __esp-idf-v5.4__ 文件夹中;

2. 在 __esp-idf-v5.4__ 目录下, 执行 `git apply --reject esp_idf_websocket_client.patch` 等待成功执行补丁;

    ```shell
    cd esp-idf-v5.4

    git apply --reject esp_idf_websocket_client.patch
    ```

3. 在 `git status` 状态下, 能检查补丁执行情况;

4. 在 idf 目录下, 执行 `git apply --reject ${adf_path}/idf_patches/idf_v5.3_freertos.patch`, 这里的 _${adf_path}_ 是 adf v2.7 的路径;

    ```shell
    # 示例, 假设 esp-adf-v2.7 和 esp-idf-v5.4 在同一目录下:
    cd esp-idf-v5.4

    git apply --reject ../esp-adf-v2.7/idf_patches/idf_v5.3_freertos.patch
    ```

5. 检查所有补丁是否正常被执行.

### 1.5 配置系统环境

建议修改 __%ADF_PATH%__ 和 __%IDF_PATH%__ 的环境变量, 分别指向 __esp-adf-v2.7__ 和 __esp-idf-v5.4__ 的路径.

## 2 工程配置

### 2.1 sdkConfig 配置

1. 搜索栏中输入 __"speech"__ 过滤配置项目;

2. 找到 __"model data path"__ 的选择项, 选择 __"Read model data from SD Card"__;

3. 勾选上 __"use afe", "use wakenet", "use "use multinet"__;

4. 找到 __"Select wake words"__ 的选择项, 选择 __你好小智(wn9_nihaoxiaozhi_tts)__;

5. 保存当前配置.

### 2.2 sdcard 文件配置

1. 在 sdcard 中, 新建一个名为 "model" 的文件夹;

2. 将 "__esp-adf-v2.7/components/esp-sr/model/wakenet_model/__" 路径下的 "__wn9_xiaobinxiaobin_tts__" 模型文件夹复制到 sdcard 的 "model" 文件夹下, 如下;

    ```shell
    sdcard/
       ├── model/
       │     └── wn9_xiaobinxiaobin_tts/
       │                └──  ...
    ```

## 3 WakeNet 模型加载

### 3.1 模型运行

见 __"./components/GT/GT_DRV/gt_wakenet.c/h"__ 文件, 该文件中包含了 __wakenet__ 模型的加载和运行.

完成.
