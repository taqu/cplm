#include <CL/cl.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <functional>

struct Platform
{
    cl_platform_id id_;
    std::string name_;
    std::string vendor_;
    std::string extensions_;
};

cl_int getPlatformInfoStr(std::string& str, cl_platform_id platform, cl_platform_info param_name)
{
    size_t value_size = 0;
    cl_int error = clGetPlatformInfo(platform, param_name, 0, nullptr, &value_size);
    if(CL_SUCCESS != error){
        return error;
    }
    str.resize(value_size);
    error = clGetPlatformInfo(platform, param_name, value_size, &str[0], &value_size);
    return error;
}

std::vector<Platform> getPlatforms(std::function<bool(const std::string& extensions)> filter)
{
    static constexpr cl_uint MaxPlatforms=4;
    std::vector<Platform> result;
    cl_int error = CL_SUCCESS;
    cl_uint numPlatforms = 0;
    cl_platform_id platforms[MaxPlatforms] = {};
    error = clGetPlatformIDs(MaxPlatforms, platforms, &numPlatforms);
    if(CL_SUCCESS != error){
        return result;
    }
    for(cl_uint i=0; i<numPlatforms; ++i){
        Platform platform;
        platform.id_ = platforms[i];
        error = getPlatformInfoStr(platform.name_, platforms[i], CL_PLATFORM_NAME);
        if(CL_SUCCESS != error){
            continue;
        }
        error = getPlatformInfoStr(platform.vendor_, platforms[i], CL_PLATFORM_VENDOR);
        if(CL_SUCCESS != error){
            continue;
        }
        error = getPlatformInfoStr(platform.extensions_, platforms[i], CL_PLATFORM_EXTENSIONS);
        if(CL_SUCCESS != error){
            continue;
        }
        if(!filter(platform.extensions_)){
            continue;
        }
        result.emplace_back(platform);
    }
    return result;
}

struct Device
{
    cl_device_id id_;
    cl_device_type type_;
    cl_uint maxComputeUnits_;
    cl_uint maxClockFrequency_;
};

Device getDevices(const std::vector<Platform>& platforms)
{
    cl_ulong performance = 0;
    Device device = {};
    for(size_t i=0; i<platforms.size(); ++i){
        cl_device_id devices[1] = {};
        cl_uint numDevices = 0;
        if(CL_SUCCESS != clGetDeviceIDs(platforms[i].id_, CL_DEVICE_TYPE_DEFAULT, 1, devices, &numDevices)){
            continue;
        }
        cl_device_type device_type;
        if(CL_SUCCESS != clGetDeviceInfo(devices[0], CL_DEVICE_TYPE, sizeof(cl_device_type), &device_type, nullptr)){
            continue;
        }
        if( CL_DEVICE_TYPE_GPU != device_type){
            continue;
        }
        cl_uint maxComputeUnits;
        if(CL_SUCCESS != clGetDeviceInfo(devices[0], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &maxComputeUnits, nullptr)){
            continue;
        }
        cl_uint maxClockFrequency;
        if(CL_SUCCESS != clGetDeviceInfo(devices[0], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &maxClockFrequency, nullptr)){
            continue;
        }
        cl_ulong perf = maxComputeUnits * maxClockFrequency;
        if(performance < perf){
            performance = perf;
            device.id_ = devices[i];
            device.type_ = device_type;
            device.maxComputeUnits_ = maxComputeUnits;
            device.maxClockFrequency_ = maxClockFrequency;
        }
    }
    return device;
}

int main(void)
{
    std::vector<Platform> platforms = getPlatforms([](const std::string& extensions)->bool{
            return std::string::npos != extensions.find("cl_khr_fp16");
    });
    for(size_t i=0; i<platforms.size(); ++i){
        printf("%s %s\n\t%s\n", platforms[i].name_.c_str(), platforms[i].vendor_.c_str(), platforms[i].extensions_.c_str());
    }
    Device device = getDevices(platforms);
    if(nullptr != device.id_){
        printf("type %lld, units: %u, freq:%u\n", device.type_, device.maxComputeUnits_, device.maxClockFrequency_);
    }
    return 0;
}

