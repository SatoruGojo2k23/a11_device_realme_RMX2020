/*
 * Copyright (C) 2018 The LineageOS Project
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
#ifndef ANDROID_HARDWARE_LIGHT_V2_0_LIGHT_H
#define ANDROID_HARDWARE_LIGHT_V2_0_LIGHT_H

#include <android/hardware/light/2.0/ILight.h>
#include <hardware/lights.h>
#include <hidl/Status.h>
#include <unordered_map>
#include <mutex>

static unsigned int brightness_table_0x7ff[256] = {
    0,    113,  218,  299,  379,  444,  484,  540,  580,  629,  669,  685,
    725,  766,  790,  830,  854,  870,  911,  935,  951,  991,  1015, 1040,
    1056, 1080, 1096, 1120, 1136, 1160, 1160, 1160, 1160, 1225, 1225, 1241,
    1265, 1265, 1281, 1281, 1305, 1305, 1322, 1322, 1346, 1346, 1362, 1362,
    1386, 1386, 1402, 1402, 1426, 1426, 1426, 1450, 1450, 1450, 1467, 1467,
    1467, 1491, 1491, 1491, 1507, 1507, 1507, 1531, 1531, 1531, 1531, 1547,
    1547, 1547, 1547, 1571, 1571, 1587, 1587, 1587, 1587, 1587, 1612, 1612,
    1612, 1636, 1636, 1636, 1636, 1636, 1652, 1652, 1652, 1652, 1652, 1676,
    1676, 1676, 1676, 1676, 1692, 1692, 1692, 1692, 1692, 1716, 1716, 1716,
    1716, 1716, 1732, 1732, 1732, 1732, 1732, 1732, 1732, 1732, 1757, 1757,
    1757, 1757, 1773, 1773, 1773, 1773, 1773, 1773, 1797, 1797, 1797, 1797,
    1797, 1797, 1797, 1797, 1797, 1797, 1813, 1813, 1813, 1813, 1813, 1813,
    1813, 1837, 1837, 1837, 1837, 1837, 1837, 1837, 1861, 1861, 1861, 1861,
    1861, 1861, 1861, 1861, 1861, 1861, 1877, 1877, 1877, 1877, 1877, 1877,
    1877, 1877, 1877, 1902, 1902, 1902, 1902, 1902, 1902, 1902, 1902, 1902,
    1918, 1918, 1918, 1918, 1918, 1918, 1918, 1918, 1918, 1918, 1942, 1942,
    1942, 1942, 1942, 1942, 1942, 1942, 1942, 1942, 1958, 1958, 1958, 1958,
    1958, 1958, 1958, 1958, 1958, 1958, 1958, 1958, 1982, 1982, 1982, 1982,
    1982, 1982, 1982, 1982, 1982, 1982, 1982, 1998, 1998, 1998, 1998, 1998,
    1998, 1998, 1998, 1998, 1998, 1998, 1998, 2022, 2022, 2022, 2022, 2022,
    2022, 2022, 2022, 2022, 2022, 2022, 2022, 2022, 2047, 2047, 2047, 2047,
    2047, 2047, 2047, 2047
};

static unsigned int brightness_table_0xfff[256] = {
    0,    226,  436,  597,  758,  887,  968,  1080, 1161, 1258, 1338, 1371,
    1451, 1532, 1580, 1661, 1709, 1741, 1822, 1870, 1902, 1983, 2031, 2080,
    2112, 2160, 2193, 2241, 2273, 2322, 2322, 2322, 2322, 2450, 2450, 2483,
    2531, 2531, 2563, 2563, 2612, 2612, 2644, 2644, 2692, 2692, 2724, 2724,
    2773, 2773, 2805, 2805, 2853, 2853, 2853, 2902, 2902, 2902, 2934, 2934,
    2934, 2982, 2982, 2982, 3015, 3015, 3015, 3063, 3063, 3063, 3063, 3095,
    3095, 3095, 3095, 3144, 3144, 3176, 3176, 3176, 3176, 3176, 3224, 3224,
    3224, 3272, 3272, 3272, 3272, 3272, 3305, 3305, 3305, 3305, 3305, 3353,
    3353, 3353, 3353, 3353, 3385, 3385, 3385, 3385, 3385, 3434, 3434, 3434,
    3434, 3434, 3466, 3466, 3466, 3466, 3466, 3466, 3466, 3466, 3514, 3514,
    3514, 3514, 3546, 3546, 3546, 3546, 3546, 3546, 3595, 3595, 3595, 3595,
    3595, 3595, 3595, 3595, 3595, 3595, 3627, 3627, 3627, 3627, 3627, 3627,
    3627, 3675, 3675, 3675, 3675, 3675, 3675, 3675, 3724, 3724, 3724, 3724,
    3724, 3724, 3724, 3724, 3724, 3724, 3756, 3756, 3756, 3756, 3756, 3756,
    3756, 3756, 3756, 3804, 3804, 3804, 3804, 3804, 3804, 3804, 3804, 3804,
    3837, 3837, 3837, 3837, 3837, 3837, 3837, 3837, 3837, 3837, 3885, 3885,
    3885, 3885, 3885, 3885, 3885, 3885, 3885, 3885, 3917, 3917, 3917, 3917,
    3917, 3917, 3917, 3917, 3917, 3917, 3917, 3917, 3966, 3966, 3966, 3966,
    3966, 3966, 3966, 3966, 3966, 3966, 3966, 3998, 3998, 3998, 3998, 3998,
    3998, 3998, 3998, 3998, 3998, 3998, 3998, 4046, 4046, 4046, 4046, 4046,
    4046, 4046, 4046, 4046, 4046, 4046, 4046, 4046, 4095, 4095, 4095, 4095,
    4095, 4095, 4095, 4095
};

namespace android {
namespace hardware {
namespace light {
namespace V2_0 {
namespace implementation {

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_vec;
using ::android::hardware::light::V2_0::ILight;
using ::android::hardware::light::V2_0::LightState;
using ::android::hardware::light::V2_0::Status;
using ::android::hardware::light::V2_0::Type;

class Light : public ILight {
  public:
    Light();

    Return<Status> setLight(Type type, const LightState& state) override;
    Return<void> getSupportedTypes(getSupportedTypes_cb _hidl_cb) override;

  private:
    void handleBacklight(const LightState& state);

    std::mutex mLock;
    std::unordered_map<Type, std::function<void(const LightState&)>> mLights;
};

}  // namespace implementation
}  // namespace V2_0
}  // namespace light
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_LIGHT_V2_0_LIGHT_H
