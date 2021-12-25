#pragma once

#include "kernel_common.h"

namespace vlr::shared {
    using DebugPayloadSignature = optixu::PayloadSignature<KernelRNG, WavelengthSamples, SampledSpectrum>;



    class BSDF {
#define VLR_MAX_NUM_BSDF_PARAMETER_SLOTS (32)
        uint32_t data[VLR_MAX_NUM_BSDF_PARAMETER_SLOTS];

        ProgSigBSDFGetBaseColor progGetBaseColor;
        ProgSigBSDFmatches progMatches;
        ProgSigBSDFSampleInternal progSampleInternal;
        ProgSigBSDFEvaluateInternal progEvaluateInternal;
        ProgSigBSDFEvaluatePDFInternal progEvaluatePDFInternal;

#if defined(VLR_Device) || defined(OPTIXU_Platform_CodeCompletion)
        CUDA_DEVICE_FUNCTION bool matches(DirectionType dirType) {
            return progMatches(reinterpret_cast<const uint32_t*>(this), dirType);
        }
        CUDA_DEVICE_FUNCTION SampledSpectrum sampleInternal(const BSDFQuery &query, float uComponent, const float uDir[2], BSDFQueryResult* result) {
            return progSampleInternal(reinterpret_cast<const uint32_t*>(this), query, uComponent, uDir, result);
        }
        CUDA_DEVICE_FUNCTION SampledSpectrum evaluateInternal(const BSDFQuery &query, const Vector3D &dirLocal) {
            return progEvaluateInternal(reinterpret_cast<const uint32_t*>(this), query, dirLocal);
        }
        CUDA_DEVICE_FUNCTION float evaluatePDFInternal(const BSDFQuery &query, const Vector3D &dirLocal) {
            return progEvaluatePDFInternal(reinterpret_cast<const uint32_t*>(this), query, dirLocal);
        }
#endif // #if defined(VLR_Device) || defined(OPTIXU_Platform_CodeCompletion)

    public:
#if defined(VLR_Device) || defined(OPTIXU_Platform_CodeCompletion)
        CUDA_DEVICE_FUNCTION BSDF(const SurfaceMaterialDescriptor &matDesc, const SurfacePoint &surfPt, const WavelengthSamples &wls) {
            auto setupBSDF = static_cast<ProgSigSetupBSDF>(matDesc.progSetupBSDF);
            setupBSDF(matDesc.data, surfPt, wls, reinterpret_cast<uint32_t*>(this));

            const BSDFProcedureSet procSet = plp.bsdfProcedureSetBuffer[matDesc.bsdfProcedureSetIndex];

            progGetBaseColor = static_cast<ProgSigBSDFGetBaseColor>(procSet.progGetBaseColor);
            progMatches = static_cast<ProgSigBSDFmatches>(procSet.progMatches);
            progSampleInternal = static_cast<ProgSigBSDFSampleInternal>(procSet.progSampleInternal);
            progEvaluateInternal = static_cast<ProgSigBSDFEvaluateInternal>(procSet.progEvaluateInternal);
            progEvaluatePDFInternal = static_cast<ProgSigBSDFEvaluatePDFInternal>(procSet.progEvaluatePDFInternal);
        }

        CUDA_DEVICE_FUNCTION SampledSpectrum getBaseColor() {
            return progGetBaseColor(reinterpret_cast<const uint32_t*>(this));
        }

        CUDA_DEVICE_FUNCTION bool hasNonDelta() {
            return matches(DirectionType::WholeSphere() | DirectionType::NonDelta());
        }

        CUDA_DEVICE_FUNCTION SampledSpectrum sample(const BSDFQuery &query, const BSDFSample &sample, BSDFQueryResult* result) {
            if (!matches(query.dirTypeFilter)) {
                result->dirPDF = 0.0f;
                result->sampledType = DirectionType();
                return SampledSpectrum::Zero();
            }
            SampledSpectrum fs_sn = sampleInternal(query, sample.uComponent, sample.uDir, result);
            VLRAssert((result->dirPDF > 0 && fs_sn.allPositiveFinite()) || result->dirPDF == 0,
                      "Invalid BSDF value.\ndirV: (%g, %g, %g), sample: (%g, %g, %g), dirPDF: %g",
                      query.dirLocal.x, query.dirLocal.y, query.dirLocal.z, sample.uComponent, sample.uDir[0], sample.uDir[1],
                      result->dirPDF);
            float snCorrection = std::fabs(result->dirLocal.z / dot(result->dirLocal, query.geometricNormalLocal));
            if (result->dirLocal.z == 0.0f)
                snCorrection = 0.0f;
            return fs_sn * snCorrection;
        }

        CUDA_DEVICE_FUNCTION SampledSpectrum evaluate(const BSDFQuery &query, const Vector3D &dirLocal) {
            SampledSpectrum fs_sn = evaluateInternal(query, dirLocal);
            float snCorrection = std::fabs(dirLocal.z / dot(dirLocal, query.geometricNormalLocal));
            if (dirLocal.z == 0.0f)
                snCorrection = 0.0f;
            return fs_sn * snCorrection;
        }

        CUDA_DEVICE_FUNCTION float evaluatePDF(const BSDFQuery &query, const Vector3D &dirLocal) {
            if (!matches(query.dirTypeFilter)) {
                return 0;
            }
            float ret = evaluatePDFInternal(query, dirLocal);
            return ret;
        }
#endif // #if defined(VLR_Device) || defined(OPTIXU_Platform_CodeCompletion)
    };



    class EDF {
#define VLR_MAX_NUM_EDF_PARAMETER_SLOTS (8)
        uint32_t data[VLR_MAX_NUM_EDF_PARAMETER_SLOTS];

        ProgSigEDFEvaluateEmittanceInternal progEvaluateEmittanceInternal;
        ProgSigEDFEvaluateInternal progEvaluateInternal;

#if defined(VLR_Device) || defined(OPTIXU_Platform_CodeCompletion)
        CUDA_DEVICE_FUNCTION SampledSpectrum evaluateEmittanceInternal() {
            return progEvaluateEmittanceInternal(reinterpret_cast<const uint32_t*>(this));
        }
        CUDA_DEVICE_FUNCTION SampledSpectrum evaluateInternal(const EDFQuery &query, const Vector3D &dirLocal) {
            return progEvaluateInternal(reinterpret_cast<const uint32_t*>(this), query, dirLocal);
        }
#endif // #if defined(VLR_Device) || defined(OPTIXU_Platform_CodeCompletion)

    public:
#if defined(VLR_Device) || defined(OPTIXU_Platform_CodeCompletion)
        CUDA_DEVICE_FUNCTION EDF(const SurfaceMaterialDescriptor &matDesc, const SurfacePoint &surfPt, const WavelengthSamples &wls) {
            auto setupEDF = static_cast<ProgSigSetupEDF>(matDesc.progSetupEDF);
            setupEDF(matDesc.data, surfPt, wls, reinterpret_cast<uint32_t*>(this));

            const EDFProcedureSet procSet = plp.edfProcedureSetBuffer[matDesc.edfProcedureSetIndex];

            progEvaluateEmittanceInternal = static_cast<ProgSigEDFEvaluateEmittanceInternal>(procSet.progEvaluateEmittanceInternal);
            progEvaluateInternal = static_cast<ProgSigEDFEvaluateInternal>(procSet.progEvaluateInternal);
        }

        CUDA_DEVICE_FUNCTION SampledSpectrum evaluateEmittance() {
            SampledSpectrum Le0 = evaluateEmittanceInternal();
            return Le0;
        }

        CUDA_DEVICE_FUNCTION SampledSpectrum evaluate(const EDFQuery &query, const Vector3D &dirLocal) {
            SampledSpectrum Le1 = evaluateInternal(query, dirLocal);
            return Le1;
        }
#endif // #if defined(VLR_Device) || defined(OPTIXU_Platform_CodeCompletion)
    };



    using ProgSigSampleLensPosition = optixu::DirectCallableProgramID<SampledSpectrum(const WavelengthSamples &, const LensPosSample &, LensPosQueryResult*)>;
    using ProgSigSampleIDF = optixu::DirectCallableProgramID<SampledSpectrum(const SurfacePoint &, const WavelengthSamples &, const IDFSample &, IDFQueryResult*)>;

    using ProgSigDecodeHitPoint = optixu::DirectCallableProgramID<void(const HitPointParameter &, SurfacePoint*, float*)>;
    using ProgSigFetchAlpha = optixu::DirectCallableProgramID<float(const TexCoord2D &)>;
    using ProgSigFetchNormal = optixu::DirectCallableProgramID<Normal3D(const TexCoord2D &)>;



#if defined(VLR_Device) || defined(OPTIXU_Platform_CodeCompletion)

    // Reference:
    // Chapter 6. A Fast and Robust Method for Avoiding Self-Intersection, Ray Tracing Gems, 2019
    CUDA_DEVICE_FUNCTION Point3D offsetRayOrigin(const Point3D &p, const Normal3D &geometricNormal) {
        constexpr float kOrigin = 1.0f / 32.0f;
        constexpr float kFloatScale = 1.0f / 65536.0f;
        constexpr float kIntScale = 256.0f;

        int32_t offsetInInt[] = {
            static_cast<int32_t>(kIntScale * geometricNormal.x),
            static_cast<int32_t>(kIntScale * geometricNormal.y),
            static_cast<int32_t>(kIntScale * geometricNormal.z)
        };

        // JP: 数学的な衝突点の座標と、実際の座標の誤差は原点からの距離に比例する。
        //     intとしてオフセットを加えることでスケール非依存に適切なオフセットを加えることができる。
        // EN: The error of the actual coorinates of the intersection point to the mathematical one is proportional to the distance to the origin.
        //     Applying the offset as int makes applying appropriate scale invariant amount of offset possible.
        Point3D newP1 = Point3D(__int_as_float(__float_as_int(p.x) + (p.x < 0 ? -1 : 1) * offsetInInt[0]),
                                __int_as_float(__float_as_int(p.y) + (p.y < 0 ? -1 : 1) * offsetInInt[1]),
                                __int_as_float(__float_as_int(p.z) + (p.z < 0 ? -1 : 1) * offsetInInt[2]));

        // JP: 原点に近い場所では、原点からの距離に依存せず一定の誤差が残るため別処理が必要。
        // EN: A constant amount of error remains near the origin independent of the distance to the origin so we need handle it separately.
        Point3D newP2 = p + kFloatScale * geometricNormal;

        return Point3D(std::fabs(p.x) < kOrigin ? newP2.x : newP1.x,
                       std::fabs(p.y) < kOrigin ? newP2.y : newP1.y,
                       std::fabs(p.z) < kOrigin ? newP2.z : newP1.z);
    }



    CUDA_DEVICE_FUNCTION float getAlpha(const WavelengthSamples &wls) {
        const auto hp = HitPointParameter::get();

        SurfacePoint surfPt;
        float hypAreaPDF;
        ProgSigDecodeHitPoint decodeHitPoint(hp.sbtr->geomInst.progDecodeHitPoint);
        decodeHitPoint(hp, &surfPt, &hypAreaPDF);
        surfPt.position = transform<TransformKind::ObjectToWorld>(surfPt.position);
        surfPt.shadingFrame = ReferenceFrame(normalize(transform<TransformKind::ObjectToWorld>(surfPt.shadingFrame.x)),
                                             normalize(transform<TransformKind::ObjectToWorld>(surfPt.shadingFrame.z)));
        surfPt.geometricNormal = normalize(transform<TransformKind::ObjectToWorld>(surfPt.geometricNormal));
        surfPt.instanceIndex = optixGetInstanceId();

        return calcNode(hp.sbtr->geomInst.nodeAlpha, 1.0f, surfPt, wls);
    }



    // JP: 変異された法線に従ってシェーディングフレームを変更する。
    // EN: perturb the shading frame according to the modified normal.
    CUDA_DEVICE_FUNCTION void applyBumpMapping(const Normal3D &modNormalInTF, SurfacePoint* surfPt) {
        if (modNormalInTF.x == 0.0f && modNormalInTF.y == 0.0f)
            return;

        // JP: 法線から回転軸と回転角(、Quaternion)を求めて対応する接平面ベクトルを求める。
        // EN: calculate a rotating axis and an angle (and quaternion) from the normal then calculate corresponding tangential vectors.
        float projLength = std::sqrt(modNormalInTF.x * modNormalInTF.x + modNormalInTF.y * modNormalInTF.y);
        float tiltAngle = std::atan(projLength / modNormalInTF.z);
        float qSin, qCos;
        ::vlr::sincos(tiltAngle / 2, &qSin, &qCos);
        float qX = (-modNormalInTF.y / projLength) * qSin;
        float qY = (modNormalInTF.x / projLength) * qSin;
        float qW = qCos;
        Vector3D modTangentInTF = Vector3D(1 - 2 * qY * qY, 2 * qX * qY, -2 * qY * qW);
        Vector3D modBitangentInTF = Vector3D(2 * qX * qY, 1 - 2 * qX * qX, 2 * qX * qW);

        Matrix3x3 matTFtoW = Matrix3x3(surfPt->shadingFrame.x, surfPt->shadingFrame.y, surfPt->shadingFrame.z);
        ReferenceFrame bumpShadingFrame(matTFtoW * modTangentInTF,
                                        matTFtoW * modBitangentInTF,
                                        matTFtoW * modNormalInTF);

        surfPt->shadingFrame = bumpShadingFrame;
    }



    // JP: 変異された接線に従ってシェーディングフレームを変更する。
    // EN: perturb the shading frame according to the modified tangent.
    CUDA_DEVICE_FUNCTION void modifyTangent(const Vector3D& modTangent, SurfacePoint* surfPt) {
        if (modTangent == surfPt->shadingFrame.x)
            return;

        float dotNT = dot(surfPt->shadingFrame.z, modTangent);
        Vector3D projModTangent = modTangent - dotNT * surfPt->shadingFrame.z;

        float lx = dot(surfPt->shadingFrame.x, projModTangent);
        float ly = dot(surfPt->shadingFrame.y, projModTangent);

        float tangentAngle = std::atan2(ly, lx);

        float s, c;
        ::vlr::sincos(tangentAngle, &s, &c);
        Vector3D modTangentInTF = Vector3D(c, s, 0);
        Vector3D modBitangentInTF = Vector3D(-s, c, 0);

        Matrix3x3 matTFtoW = Matrix3x3(surfPt->shadingFrame.x, surfPt->shadingFrame.y, surfPt->shadingFrame.z);
        ReferenceFrame newShadingFrame(normalize(matTFtoW * modTangentInTF),
                                       normalize(matTFtoW * modBitangentInTF),
                                       surfPt->shadingFrame.z);

        surfPt->shadingFrame = newShadingFrame;
    }



    CUDA_DEVICE_FUNCTION void calcSurfacePoint(
        const HitPointParameter &hp, const WavelengthSamples &wls, SurfacePoint* surfPt, float* hypAreaPDF) {
        ProgSigDecodeHitPoint decodeHitPoint(hp.sbtr->geomInst.progDecodeHitPoint);
        decodeHitPoint(hp, surfPt, hypAreaPDF);
        surfPt->position = transform<TransformKind::ObjectToWorld>(surfPt->position);
        surfPt->shadingFrame = ReferenceFrame(normalize(transform<TransformKind::ObjectToWorld>(surfPt->shadingFrame.x)),
                                              normalize(transform<TransformKind::ObjectToWorld>(surfPt->shadingFrame.z)));
        surfPt->geometricNormal = normalize(transform<TransformKind::ObjectToWorld>(surfPt->geometricNormal));
        surfPt->instanceIndex = optixGetInstanceId();

        Normal3D localNormal = calcNode(hp.sbtr->geomInst.nodeNormal, Normal3D(0.0f, 0.0f, 1.0f), *surfPt, wls);
        applyBumpMapping(localNormal, surfPt);

        Vector3D newTangent = calcNode(hp.sbtr->geomInst.nodeTangent, surfPt->shadingFrame.x, *surfPt, wls);
        modifyTangent(newTangent, surfPt);
    }

#endif // #if defined(VLR_Device) || defined(OPTIXU_Platform_CodeCompletion)
}
