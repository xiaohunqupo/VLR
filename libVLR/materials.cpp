﻿#include "materials.h"

namespace vlr {
#define BSDF_CALLABLE_NAMES(BSDFNameStr) \
    RT_DC_NAME_STR(BSDFNameStr "_getBaseColor"),\
    RT_DC_NAME_STR(BSDFNameStr "_matches"),\
    RT_DC_NAME_STR(BSDFNameStr "_sampleInternal"),\
    RT_DC_NAME_STR(BSDFNameStr "_sampleWithRevInternal"),\
    RT_DC_NAME_STR(BSDFNameStr "_evaluateInternal"),\
    RT_DC_NAME_STR(BSDFNameStr "_evaluateWithRevInternal"),\
    RT_DC_NAME_STR(BSDFNameStr "_evaluatePDFInternal"),\
    RT_DC_NAME_STR(BSDFNameStr "_evaluatePDFWithRevInternal"),\
    RT_DC_NAME_STR(BSDFNameStr "_weightInternal"),

#define EDF_CALLABLE_NAMES(EDFNameStr) \
    RT_DC_NAME_STR(EDFNameStr "_matches"),\
    RT_DC_NAME_STR(EDFNameStr "_sampleInternal"),\
    RT_DC_NAME_STR(EDFNameStr "_evaluateEmittanceInternal"),\
    RT_DC_NAME_STR(EDFNameStr "_evaluateInternal"),\
    RT_DC_NAME_STR(EDFNameStr "_evaluatePDFInternal"),\
    RT_DC_NAME_STR(EDFNameStr "_weightInternal"),\
    RT_DC_NAME_STR(EDFNameStr "_as_BSDF_getBaseColor"),\
    RT_DC_NAME_STR(EDFNameStr "_as_BSDF_matches"),\
    RT_DC_NAME_STR(EDFNameStr "_as_BSDF_sampleInternal"),\
    RT_DC_NAME_STR(EDFNameStr "_as_BSDF_sampleWithRevInternal"),\
    RT_DC_NAME_STR(EDFNameStr "_as_BSDF_evaluateInternal"),\
    RT_DC_NAME_STR(EDFNameStr "_as_BSDF_evaluateWithRevInternal"),\
    RT_DC_NAME_STR(EDFNameStr "_as_BSDF_evaluatePDFInternal"),\
    RT_DC_NAME_STR(EDFNameStr "_as_BSDF_evaluatePDFWithRevInternal"),\
    RT_DC_NAME_STR(EDFNameStr "_as_BSDF_weightInternal"),

    // static
    void SurfaceMaterial::commonInitializeProcedure(
        Context &context,
        const char* bsdfIDs[NumBSDFCallableNames], const char* edfIDs[NumEDFCallableNames],
        OptiXProgramSet* programSet) {
        if (bsdfIDs) {
            programSet->dcSetupBSDF = context.createDirectCallableProgram(
                OptiXModule_Material, bsdfIDs[BSDFCallableName_setupBSDF]);

            programSet->dcBSDFGetBaseColor = context.createDirectCallableProgram(
                OptiXModule_Material, bsdfIDs[BSDFCallableName_BSDF_getBaseColor]);
            programSet->dcBSDFmatches = context.createDirectCallableProgram(
                OptiXModule_Material, bsdfIDs[BSDFCallableName_BSDF_matches]);
            programSet->dcBSDFSampleInternal = context.createDirectCallableProgram(
                OptiXModule_Material, bsdfIDs[BSDFCallableName_BSDF_sampleInternal]);
            programSet->dcBSDFSampleWithRevInternal = context.createDirectCallableProgram(
                OptiXModule_Material, bsdfIDs[BSDFCallableName_BSDF_sampleWithRevInternal]);
            programSet->dcBSDFEvaluateInternal = context.createDirectCallableProgram(
                OptiXModule_Material, bsdfIDs[BSDFCallableName_BSDF_evaluateInternal]);
            programSet->dcBSDFEvaluateWithRevInternal = context.createDirectCallableProgram(
                OptiXModule_Material, bsdfIDs[BSDFCallableName_BSDF_evaluateWithRevInternal]);
            programSet->dcBSDFEvaluatePDFInternal = context.createDirectCallableProgram(
                OptiXModule_Material, bsdfIDs[BSDFCallableName_BSDF_evaluatePDFInternal]);
            programSet->dcBSDFEvaluatePDFWithRevInternal = context.createDirectCallableProgram(
                OptiXModule_Material, bsdfIDs[BSDFCallableName_BSDF_evaluatePDFWithRevInternal]);
            programSet->dcBSDFWeightInternal = context.createDirectCallableProgram(
                OptiXModule_Material, bsdfIDs[BSDFCallableName_BSDF_weightInternal]);

            shared::BSDFProcedureSet bsdfProcSet;
            {
                bsdfProcSet.progGetBaseColor = programSet->dcBSDFGetBaseColor;
                bsdfProcSet.progMatches = programSet->dcBSDFmatches;
                bsdfProcSet.progSampleInternal = programSet->dcBSDFSampleInternal;
                bsdfProcSet.progSampleWithRevInternal = programSet->dcBSDFSampleWithRevInternal;
                bsdfProcSet.progEvaluateInternal = programSet->dcBSDFEvaluateInternal;
                bsdfProcSet.progEvaluateWithRevInternal = programSet->dcBSDFEvaluateWithRevInternal;
                bsdfProcSet.progEvaluatePDFInternal = programSet->dcBSDFEvaluatePDFInternal;
                bsdfProcSet.progEvaluatePDFWithRevInternal = programSet->dcBSDFEvaluatePDFWithRevInternal;
                bsdfProcSet.progWeightInternal = programSet->dcBSDFWeightInternal;
            }
            programSet->bsdfProcedureSetIndex = context.allocateBSDFProcedureSet();
            context.updateBSDFProcedureSet(programSet->bsdfProcedureSetIndex, bsdfProcSet, 0);
        }

        if (edfIDs) {
            programSet->dcSetupEDF = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_setupEDF]);

            programSet->dcEDFmatches = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_EDF_matches]);
            programSet->dcEDFSampleInternal = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_EDF_sampleInternal]);
            programSet->dcEDFEvaluateEmittanceInternal = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_EDF_evaluateEmittanceInternal]);
            programSet->dcEDFEvaluateInternal = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_EDF_evaluateInternal]);
            programSet->dcEDFEvaluatePDFInternal = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_EDF_evaluatePDFInternal]);
            programSet->dcEDFWeightInternal = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_EDF_weightInternal]);

            programSet->dcEDFAsBSDFGetBaseColor = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_EDF_as_BSDF_getBaseColor]);
            programSet->dcEDFAsBSDFmatches = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_EDF_as_BSDF_matches]);
            programSet->dcEDFAsBSDFSampleInternal = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_EDF_as_BSDF_sampleInternal]);
            programSet->dcEDFAsBSDFSampleWithRevInternal = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_EDF_as_BSDF_sampleWithRevInternal]);
            programSet->dcEDFAsBSDFEvaluateInternal = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_EDF_as_BSDF_evaluateInternal]);
            programSet->dcEDFAsBSDFEvaluateWithRevInternal = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_EDF_as_BSDF_evaluateWithRevInternal]);
            programSet->dcEDFAsBSDFEvaluatePDFInternal = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_EDF_as_BSDF_evaluatePDFInternal]);
            programSet->dcEDFAsBSDFEvaluatePDFWithRevInternal = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_EDF_as_BSDF_evaluatePDFWithRevInternal]);
            programSet->dcEDFAsBSDFWeightInternal = context.createDirectCallableProgram(
                OptiXModule_Material, edfIDs[EDFCallableName_EDF_as_BSDF_weightInternal]);

            shared::EDFProcedureSet edfProcSet;
            {
                edfProcSet.progMatches = programSet->dcEDFmatches;
                edfProcSet.progSampleInternal = programSet->dcEDFSampleInternal;
                edfProcSet.progEvaluateEmittanceInternal = programSet->dcEDFEvaluateEmittanceInternal;
                edfProcSet.progEvaluateInternal = programSet->dcEDFEvaluateInternal;
                edfProcSet.progEvaluatePDFInternal = programSet->dcEDFEvaluatePDFInternal;
                edfProcSet.progWeightInternal = programSet->dcEDFWeightInternal;

                edfProcSet.progGetBaseColorAsBSDF = programSet->dcEDFAsBSDFGetBaseColor;
                edfProcSet.progMatchesAsBSDF = programSet->dcEDFAsBSDFmatches;
                edfProcSet.progSampleInternalAsBSDF = programSet->dcEDFAsBSDFSampleInternal;
                edfProcSet.progSampleWithRevInternalAsBSDF = programSet->dcEDFAsBSDFSampleWithRevInternal;
                edfProcSet.progEvaluateInternalAsBSDF = programSet->dcEDFAsBSDFEvaluateInternal;
                edfProcSet.progEvaluateWithRevInternalAsBSDF = programSet->dcEDFAsBSDFEvaluateWithRevInternal;
                edfProcSet.progEvaluatePDFInternalAsBSDF = programSet->dcEDFAsBSDFEvaluatePDFInternal;
                edfProcSet.progEvaluatePDFWithRevInternalAsBSDF = programSet->dcEDFAsBSDFEvaluatePDFWithRevInternal;
                edfProcSet.progWeightInternalAsBSDF = programSet->dcEDFAsBSDFWeightInternal;
            }
            programSet->edfProcedureSetIndex = context.allocateEDFProcedureSet();
            context.updateEDFProcedureSet(programSet->edfProcedureSetIndex, edfProcSet, 0);
        }
    }

    // static
    void SurfaceMaterial::commonFinalizeProcedure(
        Context &context, OptiXProgramSet &programSet) {
        if (programSet.dcSetupEDF) {
            context.releaseEDFProcedureSet(programSet.edfProcedureSetIndex);

            if (programSet.dcEDFEvaluateInternal != 0xFFFFFFF)
                context.destroyDirectCallableProgram(programSet.dcEDFEvaluateInternal);
            if (programSet.dcEDFEvaluateEmittanceInternal != 0xFFFFFFF)
                context.destroyDirectCallableProgram(programSet.dcEDFEvaluateEmittanceInternal);

            if (programSet.dcSetupEDF != 0xFFFFFFF)
                context.destroyDirectCallableProgram(programSet.dcSetupEDF);
        }

        if (programSet.dcSetupBSDF) {
            context.releaseBSDFProcedureSet(programSet.bsdfProcedureSetIndex);

            if (programSet.dcBSDFWeightInternal != 0xFFFFFFFF)
                context.destroyDirectCallableProgram(programSet.dcBSDFWeightInternal);
            if (programSet.dcBSDFEvaluatePDFInternal != 0xFFFFFFFF)
                context.destroyDirectCallableProgram(programSet.dcBSDFEvaluatePDFInternal);
            if (programSet.dcBSDFEvaluateInternal != 0xFFFFFFFF)
                context.destroyDirectCallableProgram(programSet.dcBSDFEvaluateInternal);
            if (programSet.dcBSDFSampleInternal != 0xFFFFFFFF)
                context.destroyDirectCallableProgram(programSet.dcBSDFSampleInternal);
            if (programSet.dcBSDFmatches != 0xFFFFFFFF)
                context.destroyDirectCallableProgram(programSet.dcBSDFmatches);
            if (programSet.dcBSDFGetBaseColor != 0xFFFFFFFF)
                context.destroyDirectCallableProgram(programSet.dcBSDFGetBaseColor);

            if (programSet.dcSetupBSDF != 0xFFFFFFFF)
                context.destroyDirectCallableProgram(programSet.dcSetupBSDF);
        }
    }

    // static
    void SurfaceMaterial::setupMaterialDescriptorHead(
        Context &context, const OptiXProgramSet &progSet, shared::SurfaceMaterialDescriptor* matDesc) {
        if (progSet.dcSetupBSDF) {
            matDesc->progSetupBSDF = progSet.dcSetupBSDF;
            matDesc->bsdfProcedureSetIndex = progSet.bsdfProcedureSetIndex;
        }
        else {
            matDesc->progSetupBSDF = context.getOptixCallableProgramNullBSDF_setupBSDF();
            matDesc->bsdfProcedureSetIndex = context.getNullBSDFProcedureSetIndex();
        }

        if (progSet.dcSetupEDF) {
            matDesc->progSetupEDF = progSet.dcSetupEDF;
            matDesc->edfProcedureSetIndex = progSet.edfProcedureSetIndex;
        }
        else {
            matDesc->progSetupEDF = context.getOptixCallableProgramNullEDF_setupEDF();
            matDesc->edfProcedureSetIndex = context.getNullEDFProcedureSetIndex();
        }
    }

    // static
    void SurfaceMaterial::initialize(Context &context) {
        MatteSurfaceMaterial::initialize(context);
        SpecularReflectionSurfaceMaterial::initialize(context);
        SpecularScatteringSurfaceMaterial::initialize(context);
        MicrofacetReflectionSurfaceMaterial::initialize(context);
        MicrofacetScatteringSurfaceMaterial::initialize(context);
        LambertianScatteringSurfaceMaterial::initialize(context);
        UE4SurfaceMaterial::initialize(context);
        OldStyleSurfaceMaterial::initialize(context);
        DiffuseEmitterSurfaceMaterial::initialize(context);
        DirectionalEmitterSurfaceMaterial::initialize(context);
        PointEmitterSurfaceMaterial::initialize(context);
        MultiSurfaceMaterial::initialize(context);
        EnvironmentEmitterSurfaceMaterial::initialize(context);
    }

    // static
    void SurfaceMaterial::finalize(Context &context) {
        EnvironmentEmitterSurfaceMaterial::finalize(context);
        MultiSurfaceMaterial::finalize(context);
        PointEmitterSurfaceMaterial::finalize(context);
        DirectionalEmitterSurfaceMaterial::finalize(context);
        DiffuseEmitterSurfaceMaterial::finalize(context);
        OldStyleSurfaceMaterial::finalize(context);
        UE4SurfaceMaterial::finalize(context);
        LambertianScatteringSurfaceMaterial::finalize(context);
        MicrofacetScatteringSurfaceMaterial::finalize(context);
        MicrofacetReflectionSurfaceMaterial::finalize(context);
        SpecularScatteringSurfaceMaterial::finalize(context);
        SpecularReflectionSurfaceMaterial::finalize(context);
        MatteSurfaceMaterial::finalize(context);
    }

    SurfaceMaterial::SurfaceMaterial(Context &context) : Queryable(context) {
        m_matIndex = m_context.allocateSurfaceMaterialDescriptor();
    }

    SurfaceMaterial::~SurfaceMaterial() {
        if (m_matIndex != 0xFFFFFFFF)
            m_context.releaseSurfaceMaterialDescriptor(m_matIndex);
        m_matIndex = 0xFFFFFFFF;
    }



    std::vector<ParameterInfo> MatteSurfaceMaterial::ParameterInfos;
    
    std::unordered_map<uint32_t, SurfaceMaterial::OptiXProgramSet> MatteSurfaceMaterial::s_optiXProgramSets;

    // static
    void MatteSurfaceMaterial::initialize(Context &context) {
        const ParameterInfo paramInfos[] = {
            ParameterInfo("albedo", VLRParameterFormFlag_Both, ParameterSpectrum),
        };

        if (ParameterInfos.size() == 0) {
            ParameterInfos.resize(lengthof(paramInfos));
            std::copy_n(paramInfos, lengthof(paramInfos), ParameterInfos.data());
        }

        const char* identifiers[] = {
            RT_DC_NAME_STR("MatteSurfaceMaterial_setupBSDF"),
            BSDF_CALLABLE_NAMES("MatteBRDF")
        };
        OptiXProgramSet programSet;
        commonInitializeProcedure(context, identifiers, nullptr, &programSet);

        s_optiXProgramSets[context.getID()] = programSet;
    }

    // static
    void MatteSurfaceMaterial::finalize(Context &context) {
        OptiXProgramSet &programSet = s_optiXProgramSets.at(context.getID());
        commonFinalizeProcedure(context, programSet);
        s_optiXProgramSets.erase(context.getID());
    }

    MatteSurfaceMaterial::MatteSurfaceMaterial(Context &context) :
        SurfaceMaterial(context), m_immAlbedo(ColorSpace::Rec709_D65, 0.18f, 0.18f, 0.18f) {
        m_context.markSurfaceMaterialDescriptorDirty(this);
    }

    MatteSurfaceMaterial::~MatteSurfaceMaterial() {
    }

    void MatteSurfaceMaterial::setupMaterialDescriptor(CUstream stream) const {
        OptiXProgramSet &progSet = s_optiXProgramSets.at(m_context.getID());

        shared::SurfaceMaterialDescriptor matDesc;
        setupMaterialDescriptorHead(m_context, progSet, &matDesc);
        auto &mat = *matDesc.getData<shared::MatteSurfaceMaterial>();
        mat.nodeAlbedo = m_nodeAlbedo.getSharedType();
        mat.immAlbedo = m_immAlbedo.createTripletSpectrum(SpectrumType::Reflectance);

        m_context.updateSurfaceMaterialDescriptor(m_matIndex, matDesc, stream);
    }

    bool MatteSurfaceMaterial::get(const char* paramName, ImmediateSpectrum* spectrum) const {
        if (spectrum == nullptr)
            return false;

        if (testParamName(paramName, "albedo")) {
            *spectrum = m_immAlbedo;
        }
        else {
            return false;
        }

        return true;
    }

    bool MatteSurfaceMaterial::get(const char* paramName, ShaderNodePlug* plug) const {
        if (plug == nullptr)
            return false;

        if (testParamName(paramName, "albedo")) {
            *plug = m_nodeAlbedo;
        }
        else {
            return false;
        }

        return true;
    }

    bool MatteSurfaceMaterial::set(const char* paramName, const ImmediateSpectrum& spectrum) {
        if (testParamName(paramName, "albedo")) {
            m_immAlbedo = spectrum;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool MatteSurfaceMaterial::set(const char* paramName, const ShaderNodePlug& plug) {
        if (testParamName(paramName, "albedo")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeAlbedo = plug;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }



    std::vector<ParameterInfo> SpecularReflectionSurfaceMaterial::ParameterInfos;

    std::unordered_map<uint32_t, SurfaceMaterial::OptiXProgramSet> SpecularReflectionSurfaceMaterial::s_optiXProgramSets;

    // static
    void SpecularReflectionSurfaceMaterial::initialize(Context &context) {
        const ParameterInfo paramInfos[] = {
            ParameterInfo("coeff", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("eta", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("k", VLRParameterFormFlag_Both, ParameterSpectrum),
        };

        if (ParameterInfos.size() == 0) {
            ParameterInfos.resize(lengthof(paramInfos));
            std::copy_n(paramInfos, lengthof(paramInfos), ParameterInfos.data());
        }

        const char* identifiers[] = {
            RT_DC_NAME_STR("SpecularReflectionSurfaceMaterial_setupBSDF"),
            BSDF_CALLABLE_NAMES("SpecularBRDF")
        };
        OptiXProgramSet programSet;
        commonInitializeProcedure(context, identifiers, nullptr, &programSet);

        s_optiXProgramSets[context.getID()] = programSet;
    }

    // static
    void SpecularReflectionSurfaceMaterial::finalize(Context &context) {
        OptiXProgramSet &programSet = s_optiXProgramSets.at(context.getID());
        commonFinalizeProcedure(context, programSet);
        s_optiXProgramSets.erase(context.getID());
    }

    SpecularReflectionSurfaceMaterial::SpecularReflectionSurfaceMaterial(Context &context) :
        SurfaceMaterial(context),
        m_immCoeff(ColorSpace::Rec709_D65, 0.8f, 0.8f, 0.8f),
        m_immEta(ColorSpace::Rec709_D65, 1.0f, 1.0f, 1.0f),
        m_imm_k(ColorSpace::Rec709_D65, 0.0f, 0.0f, 0.0f) {
        m_context.markSurfaceMaterialDescriptorDirty(this);
    }

    SpecularReflectionSurfaceMaterial::~SpecularReflectionSurfaceMaterial() {
    }

    void SpecularReflectionSurfaceMaterial::setupMaterialDescriptor(CUstream stream) const {
        OptiXProgramSet &progSet = s_optiXProgramSets.at(m_context.getID());

        shared::SurfaceMaterialDescriptor matDesc;
        setupMaterialDescriptorHead(m_context, progSet, &matDesc);
        auto &mat = *matDesc.getData<shared::SpecularReflectionSurfaceMaterial>();
        mat.nodeCoeffR = m_nodeCoeff.getSharedType();
        mat.nodeEta = m_nodeEta.getSharedType();
        mat.node_k = m_node_k.getSharedType();
        mat.immCoeffR = m_immCoeff.createTripletSpectrum(SpectrumType::Reflectance);
        mat.immEta = m_immEta.createTripletSpectrum(SpectrumType::IndexOfRefraction);
        mat.imm_k = m_imm_k.createTripletSpectrum(SpectrumType::IndexOfRefraction);

        m_context.updateSurfaceMaterialDescriptor(m_matIndex, matDesc, stream);
    }

    bool SpecularReflectionSurfaceMaterial::get(const char* paramName, ImmediateSpectrum* spectrum) const {
        if (spectrum == nullptr)
            return false;

        if (testParamName(paramName, "coeff")) {
            *spectrum = m_immCoeff;
        }
        else if (testParamName(paramName, "eta")) {
            *spectrum = m_immEta;
        }
        else if (testParamName(paramName, "k")) {
            *spectrum = m_imm_k;
        }
        else {
            return false;
        }

        return true;
    }

    bool SpecularReflectionSurfaceMaterial::get(const char* paramName, ShaderNodePlug* plug) const {
        if (plug == nullptr)
            return false;

        if (testParamName(paramName, "coeff")) {
            *plug = m_nodeCoeff;
        }
        else if (testParamName(paramName, "eta")) {
            *plug = m_nodeEta;
        }
        else if (testParamName(paramName, "k")) {
            *plug = m_node_k;
        }
        else {
            return false;
        }

        return true;
    }

    bool SpecularReflectionSurfaceMaterial::set(const char* paramName, const ImmediateSpectrum& spectrum) {
        if (testParamName(paramName, "coeff")) {
            m_immCoeff = spectrum;
        }
        else if (testParamName(paramName, "eta")) {
            m_immEta = spectrum;
        }
        else if (testParamName(paramName, "k")) {
            m_imm_k = spectrum;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool SpecularReflectionSurfaceMaterial::set(const char* paramName, const ShaderNodePlug& plug) {
        if (testParamName(paramName, "coeff")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeCoeff = plug;
        }
        else if (testParamName(paramName, "eta")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeEta = plug;
        }
        else if (testParamName(paramName, "k")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_node_k = plug;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }



    std::vector<ParameterInfo> SpecularScatteringSurfaceMaterial::ParameterInfos;

    std::unordered_map<uint32_t, SurfaceMaterial::OptiXProgramSet> SpecularScatteringSurfaceMaterial::s_optiXProgramSets;

    // static
    void SpecularScatteringSurfaceMaterial::initialize(Context &context) {
        const ParameterInfo paramInfos[] = {
            ParameterInfo("coeff", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("eta ext", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("eta int", VLRParameterFormFlag_Both, ParameterSpectrum),
        };

        if (ParameterInfos.size() == 0) {
            ParameterInfos.resize(lengthof(paramInfos));
            std::copy_n(paramInfos, lengthof(paramInfos), ParameterInfos.data());
        }

        const char* identifiers[] = {
            RT_DC_NAME_STR("SpecularScatteringSurfaceMaterial_setupBSDF"),
            BSDF_CALLABLE_NAMES("SpecularBSDF")
        };
        OptiXProgramSet programSet;
        commonInitializeProcedure(context, identifiers, nullptr, &programSet);

        s_optiXProgramSets[context.getID()] = programSet;
    }

    // static
    void SpecularScatteringSurfaceMaterial::finalize(Context &context) {
        OptiXProgramSet &programSet = s_optiXProgramSets.at(context.getID());
        commonFinalizeProcedure(context, programSet);
        s_optiXProgramSets.erase(context.getID());
    }

    SpecularScatteringSurfaceMaterial::SpecularScatteringSurfaceMaterial(Context &context) :
        SurfaceMaterial(context),
        m_immCoeff(ColorSpace::Rec709_D65, 0.8f, 0.8f, 0.8f),
        m_immEtaExt(ColorSpace::Rec709_D65, 1.0f, 1.0f, 1.0f),
        m_immEtaInt(ColorSpace::Rec709_D65, 1.5f, 1.5f, 1.5f) {
        m_context.markSurfaceMaterialDescriptorDirty(this);
    }

    SpecularScatteringSurfaceMaterial::~SpecularScatteringSurfaceMaterial() {
    }

    void SpecularScatteringSurfaceMaterial::setupMaterialDescriptor(CUstream stream) const {
        OptiXProgramSet &progSet = s_optiXProgramSets.at(m_context.getID());

        shared::SurfaceMaterialDescriptor matDesc;
        setupMaterialDescriptorHead(m_context, progSet, &matDesc);
        auto &mat = *matDesc.getData<shared::SpecularScatteringSurfaceMaterial>();
        mat.nodeCoeff = m_nodeCoeff.getSharedType();
        mat.nodeEtaExt = m_nodeEtaExt.getSharedType();
        mat.nodeEtaInt = m_nodeEtaInt.getSharedType();
        mat.immCoeff = m_immCoeff.createTripletSpectrum(SpectrumType::Reflectance);
        mat.immEtaExt = m_immEtaExt.createTripletSpectrum(SpectrumType::IndexOfRefraction);
        mat.immEtaInt = m_immEtaInt.createTripletSpectrum(SpectrumType::IndexOfRefraction);

        m_context.updateSurfaceMaterialDescriptor(m_matIndex, matDesc, stream);
    }

    bool SpecularScatteringSurfaceMaterial::get(const char* paramName, ImmediateSpectrum* spectrum) const {
        if (spectrum == nullptr)
            return false;

        if (testParamName(paramName, "coeff")) {
            *spectrum = m_immCoeff;
        }
        else if (testParamName(paramName, "eta ext")) {
            *spectrum = m_immEtaExt;
        }
        else if (testParamName(paramName, "eta int")) {
            *spectrum = m_immEtaInt;
        }
        else {
            return false;
        }

        return true;
    }

    bool SpecularScatteringSurfaceMaterial::get(const char* paramName, ShaderNodePlug* plug) const {
        if (plug == nullptr)
            return false;

        if (testParamName(paramName, "coeff")) {
            *plug = m_nodeCoeff;
        }
        else if (testParamName(paramName, "eta ext")) {
            *plug = m_nodeEtaExt;
        }
        else if (testParamName(paramName, "eta int")) {
            *plug = m_nodeEtaInt;
        }
        else {
            return false;
        }

        return true;
    }

    bool SpecularScatteringSurfaceMaterial::set(const char* paramName, const ImmediateSpectrum& spectrum) {
        if (testParamName(paramName, "coeff")) {
            m_immCoeff = spectrum;
        }
        else if (testParamName(paramName, "eta ext")) {
            m_immEtaExt = spectrum;
        }
        else if (testParamName(paramName, "eta int")) {
            m_immEtaInt = spectrum;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool SpecularScatteringSurfaceMaterial::set(const char* paramName, const ShaderNodePlug& plug) {
        if (testParamName(paramName, "coeff")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeCoeff = plug;
        }
        else if (testParamName(paramName, "eta ext")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeEtaExt = plug;
        }
        else if (testParamName(paramName, "eta int")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeEtaInt = plug;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }



    std::vector<ParameterInfo> MicrofacetReflectionSurfaceMaterial::ParameterInfos;

    std::unordered_map<uint32_t, SurfaceMaterial::OptiXProgramSet> MicrofacetReflectionSurfaceMaterial::s_optiXProgramSets;

    // static
    void MicrofacetReflectionSurfaceMaterial::initialize(Context &context) {
        const ParameterInfo paramInfos[] = {
            ParameterInfo("eta", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("k", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("roughness/anisotropy/rotation", VLRParameterFormFlag_Node, ParameterFloat, 3),
            ParameterInfo("roughness", VLRParameterFormFlag_ImmediateValue, ParameterFloat),
            ParameterInfo("anisotropy", VLRParameterFormFlag_ImmediateValue, ParameterFloat),
            ParameterInfo("rotation", VLRParameterFormFlag_ImmediateValue, ParameterFloat),
        };

        if (ParameterInfos.size() == 0) {
            ParameterInfos.resize(lengthof(paramInfos));
            std::copy_n(paramInfos, lengthof(paramInfos), ParameterInfos.data());
        }

        const char* identifiers[] = {
            RT_DC_NAME_STR("MicrofacetReflectionSurfaceMaterial_setupBSDF"),
            BSDF_CALLABLE_NAMES("MicrofacetBRDF")
        };
        OptiXProgramSet programSet;
        commonInitializeProcedure(context, identifiers, nullptr, &programSet);

        s_optiXProgramSets[context.getID()] = programSet;
    }

    // static
    void MicrofacetReflectionSurfaceMaterial::finalize(Context &context) {
        OptiXProgramSet &programSet = s_optiXProgramSets.at(context.getID());
        commonFinalizeProcedure(context, programSet);
        s_optiXProgramSets.erase(context.getID());
    }

    MicrofacetReflectionSurfaceMaterial::MicrofacetReflectionSurfaceMaterial(Context &context) :
        SurfaceMaterial(context),
        m_immEta(ColorSpace::Rec709_D65, 1.0f, 1.0f, 1.0f),
        m_imm_k(ColorSpace::Rec709_D65, 0.0f, 0.0f, 0.0f),
        m_immRoughness(0.1f), m_immAnisotropy(0.0f), m_immRotation(0.0f) {
        m_context.markSurfaceMaterialDescriptorDirty(this);
    }

    MicrofacetReflectionSurfaceMaterial::~MicrofacetReflectionSurfaceMaterial() {
    }

    void MicrofacetReflectionSurfaceMaterial::setupMaterialDescriptor(CUstream stream) const {
        OptiXProgramSet &progSet = s_optiXProgramSets.at(m_context.getID());

        shared::SurfaceMaterialDescriptor matDesc;
        setupMaterialDescriptorHead(m_context, progSet, &matDesc);
        auto &mat = *matDesc.getData<shared::MicrofacetReflectionSurfaceMaterial>();
        mat.nodeEta = m_nodeEta.getSharedType();
        mat.node_k = m_node_k.getSharedType();
        mat.nodeRoughnessAnisotropyRotation = m_nodeRoughnessAnisotropyRotation.getSharedType();
        mat.immEta = m_immEta.createTripletSpectrum(SpectrumType::IndexOfRefraction);
        mat.imm_k = m_imm_k.createTripletSpectrum(SpectrumType::IndexOfRefraction);
        mat.immRoughness = m_immRoughness;
        mat.immAnisotropy = m_immAnisotropy;
        mat.immRotation = m_immRotation;

        m_context.updateSurfaceMaterialDescriptor(m_matIndex, matDesc, stream);
    }

    bool MicrofacetReflectionSurfaceMaterial::get(const char* paramName, float* values, uint32_t length) const {
        if (values == nullptr)
            return false;

        if (testParamName(paramName, "roughness")) {
            if (length != 1)
                return false;

            values[0] = m_immRoughness;
        }
        else if (testParamName(paramName, "anisotropy")) {
            if (length != 1)
                return false;

            values[0] = m_immAnisotropy;
        }
        else if (testParamName(paramName, "rotation")) {
            if (length != 1)
                return false;

            values[0] = m_immRotation;
        }
        else {
            return false;
        }

        return true;
    }

    bool MicrofacetReflectionSurfaceMaterial::get(const char* paramName, ImmediateSpectrum* spectrum) const {
        if (spectrum == nullptr)
            return false;

        if (testParamName(paramName, "eta")) {
            *spectrum = m_immEta;
        }
        else if (testParamName(paramName, "k")) {
            *spectrum = m_imm_k;
        }
        else {
            return false;
        }

        return true;
    }

    bool MicrofacetReflectionSurfaceMaterial::get(const char* paramName, ShaderNodePlug* plug) const {
        if (plug == nullptr)
            return false;

        if (testParamName(paramName, "eta")) {
            *plug = m_nodeEta;
        }
        else if (testParamName(paramName, "k")) {
            *plug = m_node_k;
        }
        else if (testParamName(paramName, "roughness/anisotropy/rotation")) {
            *plug = m_nodeRoughnessAnisotropyRotation;
        }
        else {
            return false;
        }

        return true;
    }

    bool MicrofacetReflectionSurfaceMaterial::set(const char* paramName, const float* values, uint32_t length) {
        if (testParamName(paramName, "roughness")) {
            if (length != 1)
                return false;

            m_immRoughness = values[0];
        }
        else if (testParamName(paramName, "anisotropy")) {
            if (length != 1)
                return false;

            m_immAnisotropy = values[0];
        }
        else if (testParamName(paramName, "rotation")) {
            if (length != 1)
                return false;

            m_immRotation = values[0];
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool MicrofacetReflectionSurfaceMaterial::set(const char* paramName, const ImmediateSpectrum& spectrum) {
        if (testParamName(paramName, "eta")) {
            m_immEta = spectrum;
        }
        else if (testParamName(paramName, "k")) {
            m_imm_k = spectrum;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool MicrofacetReflectionSurfaceMaterial::set(const char* paramName, const ShaderNodePlug& plug) {
        if (testParamName(paramName, "eta")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeEta = plug;
        }
        else if (testParamName(paramName, "k")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_node_k = plug;
        }
        else if (testParamName(paramName, "roughness/anisotropy/rotation")) {
            if (!shared::NodeTypeInfo<float3>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeRoughnessAnisotropyRotation = plug;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }



    std::vector<ParameterInfo> MicrofacetScatteringSurfaceMaterial::ParameterInfos;

    std::unordered_map<uint32_t, SurfaceMaterial::OptiXProgramSet> MicrofacetScatteringSurfaceMaterial::s_optiXProgramSets;

    // static
    void MicrofacetScatteringSurfaceMaterial::initialize(Context &context) {
        const ParameterInfo paramInfos[] = {
            ParameterInfo("coeff", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("eta ext", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("eta Int", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("roughness/anisotropy/rotation", VLRParameterFormFlag_Node, ParameterFloat, 3),
            ParameterInfo("roughness", VLRParameterFormFlag_ImmediateValue, ParameterFloat),
            ParameterInfo("anisotropy", VLRParameterFormFlag_ImmediateValue, ParameterFloat),
            ParameterInfo("rotation", VLRParameterFormFlag_ImmediateValue, ParameterFloat),
        };

        if (ParameterInfos.size() == 0) {
            ParameterInfos.resize(lengthof(paramInfos));
            std::copy_n(paramInfos, lengthof(paramInfos), ParameterInfos.data());
        }

        const char* identifiers[] = {
            RT_DC_NAME_STR("MicrofacetScatteringSurfaceMaterial_setupBSDF"),
            BSDF_CALLABLE_NAMES("MicrofacetBSDF")
        };
        OptiXProgramSet programSet;
        commonInitializeProcedure(context, identifiers, nullptr, &programSet);

        s_optiXProgramSets[context.getID()] = programSet;
    }

    // static
    void MicrofacetScatteringSurfaceMaterial::finalize(Context &context) {
        OptiXProgramSet &programSet = s_optiXProgramSets.at(context.getID());
        commonFinalizeProcedure(context, programSet);
        s_optiXProgramSets.erase(context.getID());
    }

    MicrofacetScatteringSurfaceMaterial::MicrofacetScatteringSurfaceMaterial(Context &context) :
        SurfaceMaterial(context),
        m_immCoeff(ColorSpace::Rec709_D65, 0.8f, 0.8f, 0.8f),
        m_immEtaExt(ColorSpace::Rec709_D65, 1.0f, 1.0f, 1.0f),
        m_immEtaInt(ColorSpace::Rec709_D65, 1.5f, 1.5f, 1.5f),
        m_immRoughness(0.1f), m_immAnisotropy(0.0f), m_immRotation(0.0f) {
        m_context.markSurfaceMaterialDescriptorDirty(this);
    }

    MicrofacetScatteringSurfaceMaterial::~MicrofacetScatteringSurfaceMaterial() {
    }

    void MicrofacetScatteringSurfaceMaterial::setupMaterialDescriptor(CUstream stream) const {
        OptiXProgramSet &progSet = s_optiXProgramSets.at(m_context.getID());

        shared::SurfaceMaterialDescriptor matDesc;
        setupMaterialDescriptorHead(m_context, progSet, &matDesc);
        auto &mat = *matDesc.getData<shared::MicrofacetScatteringSurfaceMaterial>();
        mat.nodeCoeff = m_nodeCoeff.getSharedType();
        mat.nodeEtaExt = m_nodeEtaExt.getSharedType();
        mat.nodeEtaInt = m_nodeEtaInt.getSharedType();
        mat.nodeRoughnessAnisotropyRotation = m_nodeRoughnessAnisotropyRotation.getSharedType();
        mat.immCoeff = m_immCoeff.createTripletSpectrum(SpectrumType::Reflectance);
        mat.immEtaExt = m_immEtaExt.createTripletSpectrum(SpectrumType::IndexOfRefraction);
        mat.immEtaInt = m_immEtaInt.createTripletSpectrum(SpectrumType::IndexOfRefraction);
        mat.immRoughness = m_immRoughness;
        mat.immAnisotropy = m_immAnisotropy;
        mat.immRotation = m_immRotation;

        m_context.updateSurfaceMaterialDescriptor(m_matIndex, matDesc, stream);
    }

    bool MicrofacetScatteringSurfaceMaterial::get(const char* paramName, float* values, uint32_t length) const {
        if (values == nullptr)
            return false;

        if (testParamName(paramName, "roughness")) {
            if (length != 1)
                return false;

            values[0] = m_immRoughness;
        }
        else if (testParamName(paramName, "anisotropy")) {
            if (length != 1)
                return false;

            values[0] = m_immAnisotropy;
        }
        else if (testParamName(paramName, "rotation")) {
            if (length != 1)
                return false;

            values[0] = m_immRotation;
        }
        else {
            return false;
        }

        return true;
    }

    bool MicrofacetScatteringSurfaceMaterial::get(const char* paramName, ImmediateSpectrum* spectrum) const {
        if (spectrum == nullptr)
            return false;

        if (testParamName(paramName, "coeff")) {
            *spectrum = m_immCoeff;
        }
        else if (testParamName(paramName, "eta ext")) {
            *spectrum = m_immEtaExt;
        }
        else if (testParamName(paramName, "eta int")) {
            *spectrum = m_immEtaInt;
        }
        else {
            return false;
        }

        return true;
    }

    bool MicrofacetScatteringSurfaceMaterial::get(const char* paramName, ShaderNodePlug* plug) const {
        if (plug == nullptr)
            return false;

        if (testParamName(paramName, "coeff")) {
            *plug = m_nodeCoeff;
        }
        else if (testParamName(paramName, "eta ext")) {
            *plug = m_nodeEtaExt;
        }
        else if (testParamName(paramName, "eta int")) {
            *plug = m_nodeEtaInt;
        }
        else if (testParamName(paramName, "roughness/anisotropy/rotation")) {
            *plug = m_nodeRoughnessAnisotropyRotation;
        }
        else {
            return false;
        }

        return true;
    }

    bool MicrofacetScatteringSurfaceMaterial::set(const char* paramName, const float* values, uint32_t length) {
        if (testParamName(paramName, "roughness")) {
            if (length != 1)
                return false;

            m_immRoughness = values[0];
        }
        else if (testParamName(paramName, "anisotropy")) {
            if (length != 1)
                return false;

            m_immAnisotropy = values[0];
        }
        else if (testParamName(paramName, "rotation")) {
            if (length != 1)
                return false;

            m_immRotation = values[0];
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool MicrofacetScatteringSurfaceMaterial::set(const char* paramName, const ImmediateSpectrum& spectrum) {
        if (testParamName(paramName, "coeff")) {
            m_immCoeff = spectrum;
        }
        else if (testParamName(paramName, "eta ext")) {
            m_immEtaExt = spectrum;
        }
        else if (testParamName(paramName, "eta int")) {
            m_immEtaInt = spectrum;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool MicrofacetScatteringSurfaceMaterial::set(const char* paramName, const ShaderNodePlug& plug) {
        if (testParamName(paramName, "coeff")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeCoeff = plug;
        }
        else if (testParamName(paramName, "eta ext")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeEtaExt = plug;
        }
        else if (testParamName(paramName, "eta int")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeEtaInt = plug;
        }
        else if (testParamName(paramName, "roughness/anisotropy/rotation")) {
            if (!shared::NodeTypeInfo<float3>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeRoughnessAnisotropyRotation = plug;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }



    std::vector<ParameterInfo> LambertianScatteringSurfaceMaterial::ParameterInfos;

    std::unordered_map<uint32_t, SurfaceMaterial::OptiXProgramSet> LambertianScatteringSurfaceMaterial::s_optiXProgramSets;

    // static
    void LambertianScatteringSurfaceMaterial::initialize(Context &context) {
        const ParameterInfo paramInfos[] = {
            ParameterInfo("coeff", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("f0", VLRParameterFormFlag_Both, ParameterFloat),
        };

        if (ParameterInfos.size() == 0) {
            ParameterInfos.resize(lengthof(paramInfos));
            std::copy_n(paramInfos, lengthof(paramInfos), ParameterInfos.data());
        }

        const char* identifiers[] = {
            RT_DC_NAME_STR("LambertianScatteringSurfaceMaterial_setupBSDF"),
            BSDF_CALLABLE_NAMES("LambertianBSDF")
        };
        OptiXProgramSet programSet;
        commonInitializeProcedure(context, identifiers, nullptr, &programSet);

        s_optiXProgramSets[context.getID()] = programSet;
    }

    // static
    void LambertianScatteringSurfaceMaterial::finalize(Context &context) {
        OptiXProgramSet &programSet = s_optiXProgramSets.at(context.getID());
        commonFinalizeProcedure(context, programSet);
        s_optiXProgramSets.erase(context.getID());
    }

    LambertianScatteringSurfaceMaterial::LambertianScatteringSurfaceMaterial(Context &context) :
        SurfaceMaterial(context),
        m_immCoeff(ColorSpace::Rec709_D65, 0.8f, 0.8f, 0.8f), m_immF0(0.04f) {
        m_context.markSurfaceMaterialDescriptorDirty(this);
    }

    LambertianScatteringSurfaceMaterial::~LambertianScatteringSurfaceMaterial() {
    }

    void LambertianScatteringSurfaceMaterial::setupMaterialDescriptor(CUstream stream) const {
        OptiXProgramSet &progSet = s_optiXProgramSets.at(m_context.getID());

        shared::SurfaceMaterialDescriptor matDesc;
        setupMaterialDescriptorHead(m_context, progSet, &matDesc);
        auto &mat = *matDesc.getData<shared::LambertianScatteringSurfaceMaterial>();
        mat.nodeCoeff = m_nodeCoeff.getSharedType();
        mat.nodeF0 = m_nodeF0.getSharedType();
        mat.immCoeff = m_immCoeff.createTripletSpectrum(SpectrumType::Reflectance);
        mat.immF0 = m_immF0;

        m_context.updateSurfaceMaterialDescriptor(m_matIndex, matDesc, stream);
    }

    bool LambertianScatteringSurfaceMaterial::get(const char* paramName, float* values, uint32_t length) const {
        if (values == nullptr)
            return false;

        if (testParamName(paramName, "f0")) {
            if (length != 1)
                return false;

            values[0] = m_immF0;
        }
        else {
            return false;
        }

        return true;
    }

    bool LambertianScatteringSurfaceMaterial::get(const char* paramName, ImmediateSpectrum* spectrum) const {
        if (spectrum == nullptr)
            return false;

        if (testParamName(paramName, "coeff")) {
            *spectrum = m_immCoeff;
        }
        else {
            return false;
        }

        return true;
    }

    bool LambertianScatteringSurfaceMaterial::get(const char* paramName, ShaderNodePlug* plug) const {
        if (plug == nullptr)
            return false;

        if (testParamName(paramName, "coeff")) {
            *plug = m_nodeCoeff;
        }
        else if (testParamName(paramName, "f0")) {
            *plug = m_nodeF0;
        }
        else {
            return false;
        }

        return true;
    }

    bool LambertianScatteringSurfaceMaterial::set(const char* paramName, const float* values, uint32_t length) {
        if (testParamName(paramName, "f0")) {
            if (length != 1)
                return false;

            m_immF0 = values[0];
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool LambertianScatteringSurfaceMaterial::set(const char* paramName, const ImmediateSpectrum& spectrum) {
        if (testParamName(paramName, "coeff")) {
            m_immCoeff = spectrum;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool LambertianScatteringSurfaceMaterial::set(const char* paramName, const ShaderNodePlug& plug) {
        if (testParamName(paramName, "coeff")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeCoeff = plug;
        }
        else if (testParamName(paramName, "f0")) {
            if (!shared::NodeTypeInfo<float>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeF0 = plug;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }



    std::vector<ParameterInfo> UE4SurfaceMaterial::ParameterInfos;

    std::unordered_map<uint32_t, SurfaceMaterial::OptiXProgramSet> UE4SurfaceMaterial::s_optiXProgramSets;

    // static
    void UE4SurfaceMaterial::initialize(Context &context) {
        const ParameterInfo paramInfos[] = {
            ParameterInfo("base color", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("occlusion/roughness/metallic", VLRParameterFormFlag_Node, ParameterFloat, 3),
            ParameterInfo("occlusion", VLRParameterFormFlag_ImmediateValue, ParameterFloat),
            ParameterInfo("roughness", VLRParameterFormFlag_ImmediateValue, ParameterFloat),
            ParameterInfo("metallic", VLRParameterFormFlag_ImmediateValue, ParameterFloat),
        };

        if (ParameterInfos.size() == 0) {
            ParameterInfos.resize(lengthof(paramInfos));
            std::copy_n(paramInfos, lengthof(paramInfos), ParameterInfos.data());
        }

        const char* identifiers[] = {
            RT_DC_NAME_STR("UE4SurfaceMaterial_setupBSDF"),
            BSDF_CALLABLE_NAMES("DiffuseAndSpecularBRDF")
        };
        OptiXProgramSet programSet;
        commonInitializeProcedure(context, identifiers, nullptr, &programSet);

        s_optiXProgramSets[context.getID()] = programSet;
    }

    // static
    void UE4SurfaceMaterial::finalize(Context &context) {
        OptiXProgramSet &programSet = s_optiXProgramSets.at(context.getID());
        commonFinalizeProcedure(context, programSet);
        s_optiXProgramSets.erase(context.getID());
    }

    UE4SurfaceMaterial::UE4SurfaceMaterial(Context &context) :
        SurfaceMaterial(context),
        m_immBaseColor(ColorSpace::Rec709_D65, 0.18f, 0.18f, 0.18f), m_immOcculusion(0.0f), m_immRoughness(0.1f), m_immMetallic(0.0f) {
        m_context.markSurfaceMaterialDescriptorDirty(this);
    }

    UE4SurfaceMaterial::~UE4SurfaceMaterial() {
    }

    void UE4SurfaceMaterial::setupMaterialDescriptor(CUstream stream) const {
        OptiXProgramSet &progSet = s_optiXProgramSets.at(m_context.getID());

        shared::SurfaceMaterialDescriptor matDesc;
        setupMaterialDescriptorHead(m_context, progSet, &matDesc);
        auto &mat = *matDesc.getData<shared::UE4SurfaceMaterial>();
        mat.nodeBaseColor = m_nodeBaseColor.getSharedType();
        mat.nodeOcclusionRoughnessMetallic = m_nodeOcclusionRoughnessMetallic.getSharedType();
        mat.immBaseColor = m_immBaseColor.createTripletSpectrum(SpectrumType::Reflectance);
        mat.immOcclusion = m_immOcculusion;
        mat.immRoughness = m_immRoughness;
        mat.immMetallic = m_immMetallic;

        m_context.updateSurfaceMaterialDescriptor(m_matIndex, matDesc, stream);
    }

    bool UE4SurfaceMaterial::get(const char* paramName, float* values, uint32_t length) const {
        if (values == nullptr)
            return false;

        if (testParamName(paramName, "occlusion")) {
            if (length != 1)
                return false;

            values[0] = m_immOcculusion;
        }
        else if (testParamName(paramName, "roughness")) {
            if (length != 1)
                return false;

            values[0] = m_immRoughness;
        }
        else if (testParamName(paramName, "metallic")) {
            if (length != 1)
                return false;

            values[0] = m_immMetallic;
        }
        else {
            return false;
        }

        return true;
    }

    bool UE4SurfaceMaterial::get(const char* paramName, ImmediateSpectrum* spectrum) const {
        if (spectrum == nullptr)
            return false;

        if (testParamName(paramName, "base color")) {
            *spectrum = m_immBaseColor;
        }
        else {
            return false;
        }

        return true;
    }

    bool UE4SurfaceMaterial::get(const char* paramName, ShaderNodePlug* plug) const {
        if (plug == nullptr)
            return false;

        if (testParamName(paramName, "base color")) {
            *plug = m_nodeBaseColor;
        }
        else if (testParamName(paramName, "occlusion/roughness/metallic")) {
            *plug = m_nodeOcclusionRoughnessMetallic;
        }
        else {
            return false;
        }

        return true;
    }

    bool UE4SurfaceMaterial::set(const char* paramName, const float* values, uint32_t length) {
        if (testParamName(paramName, "occlusion")) {
            if (length != 1)
                return false;

            m_immOcculusion = values[0];
        }
        else if (testParamName(paramName, "roughness")) {
            if (length != 1)
                return false;

            m_immRoughness = values[0];
        }
        else if (testParamName(paramName, "metallic")) {
            if (length != 1)
                return false;

            m_immMetallic = values[0];
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool UE4SurfaceMaterial::set(const char* paramName, const ImmediateSpectrum& spectrum) {
        if (testParamName(paramName, "base color")) {
            m_immBaseColor = spectrum;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool UE4SurfaceMaterial::set(const char* paramName, const ShaderNodePlug& plug) {
        if (testParamName(paramName, "base color")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeBaseColor = plug;
        }
        else if (testParamName(paramName, "occlusion/roughness/metallic")) {
            if (!shared::NodeTypeInfo<float3>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeOcclusionRoughnessMetallic = plug;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }



    std::vector<ParameterInfo> OldStyleSurfaceMaterial::ParameterInfos;

    std::unordered_map<uint32_t, SurfaceMaterial::OptiXProgramSet> OldStyleSurfaceMaterial::s_optiXProgramSets;

    // static
    void OldStyleSurfaceMaterial::initialize(Context &context) {
        const ParameterInfo paramInfos[] = {
            ParameterInfo("diffuse", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("specular", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("glossiness", VLRParameterFormFlag_Both, ParameterFloat),
        };

        if (ParameterInfos.size() == 0) {
            ParameterInfos.resize(lengthof(paramInfos));
            std::copy_n(paramInfos, lengthof(paramInfos), ParameterInfos.data());
        }

        const char* identifiers[] = {
            RT_DC_NAME_STR("OldStyleSurfaceMaterial_setupBSDF"),
            BSDF_CALLABLE_NAMES("DiffuseAndSpecularBRDF")
        };
        OptiXProgramSet programSet;
        commonInitializeProcedure(context, identifiers, nullptr, &programSet);

        s_optiXProgramSets[context.getID()] = programSet;
    }

    // static
    void OldStyleSurfaceMaterial::finalize(Context &context) {
        OptiXProgramSet &programSet = s_optiXProgramSets.at(context.getID());
        commonFinalizeProcedure(context, programSet);
        s_optiXProgramSets.erase(context.getID());
    }

    OldStyleSurfaceMaterial::OldStyleSurfaceMaterial(Context &context) :
        SurfaceMaterial(context),
        m_immDiffuseColor(ColorSpace::Rec709_D65, 0.18f, 0.18f, 0.18f),
        m_immSpecularColor(ColorSpace::Rec709_D65, 0.04f, 0.04f, 0.04f),
        m_immGlossiness(0.6f) {
        m_context.markSurfaceMaterialDescriptorDirty(this);
    }

    OldStyleSurfaceMaterial::~OldStyleSurfaceMaterial() {
    }

    void OldStyleSurfaceMaterial::setupMaterialDescriptor(CUstream stream) const {
        OptiXProgramSet &progSet = s_optiXProgramSets.at(m_context.getID());

        shared::SurfaceMaterialDescriptor matDesc;
        setupMaterialDescriptorHead(m_context, progSet, &matDesc);
        auto &mat = *matDesc.getData<shared::OldStyleSurfaceMaterial>();
        mat.nodeDiffuseColor = m_nodeDiffuseColor.getSharedType();
        mat.nodeSpecularColor = m_nodeSpecularColor.getSharedType();
        mat.nodeGlossiness = m_nodeGlossiness.getSharedType();
        mat.immDiffuseColor = m_immDiffuseColor.createTripletSpectrum(SpectrumType::Reflectance);
        mat.immSpecularColor = m_immSpecularColor.createTripletSpectrum(SpectrumType::Reflectance);
        mat.immGlossiness = m_immGlossiness;

        m_context.updateSurfaceMaterialDescriptor(m_matIndex, matDesc, stream);
    }

    bool OldStyleSurfaceMaterial::get(const char* paramName, float* values, uint32_t length) const {
        if (values == nullptr)
            return false;

        if (testParamName(paramName, "glossiness")) {
            if (length != 1)
                return false;

            values[0] = m_immGlossiness;
        }
        else {
            return false;
        }

        return true;
    }

    bool OldStyleSurfaceMaterial::get(const char* paramName, ImmediateSpectrum* spectrum) const {
        if (spectrum == nullptr)
            return false;

        if (testParamName(paramName, "diffuse")) {
            *spectrum = m_immDiffuseColor;
        }
        else if (testParamName(paramName, "specular")) {
            *spectrum = m_immSpecularColor;
        }
        else {
            return false;
        }

        return true;
    }

    bool OldStyleSurfaceMaterial::get(const char* paramName, ShaderNodePlug* plug) const {
        if (plug == nullptr)
            return false;

        if (testParamName(paramName, "diffuse")) {
            *plug = m_nodeDiffuseColor;
        }
        else if (testParamName(paramName, "specular")) {
            *plug = m_nodeSpecularColor;
        }
        else if (testParamName(paramName, "glossiness")) {
            *plug = m_nodeGlossiness;
        }
        else {
            return false;
        }

        return true;
    }

    bool OldStyleSurfaceMaterial::set(const char* paramName, const float* values, uint32_t length) {
        if (testParamName(paramName, "glossiness")) {
            if (length != 1)
                return false;

            m_immGlossiness = values[0];
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool OldStyleSurfaceMaterial::set(const char* paramName, const ImmediateSpectrum& spectrum) {
        if (testParamName(paramName, "diffuse")) {
            m_immDiffuseColor = spectrum;
        }
        else if (testParamName(paramName, "specular")) {
            m_immSpecularColor = spectrum;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool OldStyleSurfaceMaterial::set(const char* paramName, const ShaderNodePlug& plug) {
        if (testParamName(paramName, "diffuse")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeDiffuseColor = plug;
        }
        else if (testParamName(paramName, "specular")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeSpecularColor = plug;
        }
        else if (testParamName(paramName, "glossiness")) {
            if (!shared::NodeTypeInfo<float>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeGlossiness = plug;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }



    std::vector<ParameterInfo> DiffuseEmitterSurfaceMaterial::ParameterInfos;

    std::unordered_map<uint32_t, SurfaceMaterial::OptiXProgramSet> DiffuseEmitterSurfaceMaterial::s_optiXProgramSets;

    // static
    void DiffuseEmitterSurfaceMaterial::initialize(Context &context) {
        const ParameterInfo paramInfos[] = {
            ParameterInfo("emittance", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("scale", VLRParameterFormFlag_ImmediateValue, ParameterFloat),
        };

        if (ParameterInfos.size() == 0) {
            ParameterInfos.resize(lengthof(paramInfos));
            std::copy_n(paramInfos, lengthof(paramInfos), ParameterInfos.data());
        }

        const char* identifiers[] = {
            RT_DC_NAME_STR("DiffuseEmitterSurfaceMaterial_setupEDF"),
            EDF_CALLABLE_NAMES("DiffuseEDF")
        };
        OptiXProgramSet programSet;
        commonInitializeProcedure(context, nullptr, identifiers, &programSet);

        s_optiXProgramSets[context.getID()] = programSet;
    }

    // static
    void DiffuseEmitterSurfaceMaterial::finalize(Context &context) {
        OptiXProgramSet &programSet = s_optiXProgramSets.at(context.getID());
        commonFinalizeProcedure(context, programSet);
        s_optiXProgramSets.erase(context.getID());
    }

    DiffuseEmitterSurfaceMaterial::DiffuseEmitterSurfaceMaterial(Context &context) :
        SurfaceMaterial(context), m_immEmittance(ColorSpace::Rec709_D65, M_PI, M_PI, M_PI), m_immScale(1.0f) {
        m_context.markSurfaceMaterialDescriptorDirty(this);
    }

    DiffuseEmitterSurfaceMaterial::~DiffuseEmitterSurfaceMaterial() {
    }

    void DiffuseEmitterSurfaceMaterial::setupMaterialDescriptor(CUstream stream) const {
        OptiXProgramSet &progSet = s_optiXProgramSets.at(m_context.getID());

        shared::SurfaceMaterialDescriptor matDesc;
        setupMaterialDescriptorHead(m_context, progSet, &matDesc);
        auto &mat = *matDesc.getData<shared::DiffuseEmitterSurfaceMaterial>();
        mat.nodeEmittance = m_nodeEmittance.getSharedType();
        mat.immEmittance = m_immEmittance.createTripletSpectrum(SpectrumType::LightSource);
        mat.immScale = m_immScale;

        m_context.updateSurfaceMaterialDescriptor(m_matIndex, matDesc, stream);
    }

    bool DiffuseEmitterSurfaceMaterial::get(const char* paramName, float* values, uint32_t length) const {
        if (values == nullptr)
            return false;

        if (testParamName(paramName, "scale")) {
            if (length != 1)
                return false;

            values[0] = m_immScale;
        }
        else {
            return false;
        }

        return true;
    }

    bool DiffuseEmitterSurfaceMaterial::get(const char* paramName, ImmediateSpectrum* spectrum) const {
        if (spectrum == nullptr)
            return false;

        if (testParamName(paramName, "emittance")) {
            *spectrum = m_immEmittance;
        }
        else {
            return false;
        }

        return true;
    }

    bool DiffuseEmitterSurfaceMaterial::get(const char* paramName, ShaderNodePlug* plug) const {
        if (plug == nullptr)
            return false;

        if (testParamName(paramName, "emittance")) {
            *plug = m_nodeEmittance;
        }
        else {
            return false;
        }

        return true;
    }

    bool DiffuseEmitterSurfaceMaterial::set(const char* paramName, const float* values, uint32_t length) {
        if (testParamName(paramName, "scale")) {
            if (length != 1)
                return false;

            m_immScale = values[0];
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool DiffuseEmitterSurfaceMaterial::set(const char* paramName, const ImmediateSpectrum& spectrum) {
        if (testParamName(paramName, "emittance")) {
            m_immEmittance = spectrum;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool DiffuseEmitterSurfaceMaterial::set(const char* paramName, const ShaderNodePlug& plug) {
        if (testParamName(paramName, "emittance")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeEmittance = plug;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }



    std::vector<ParameterInfo> DirectionalEmitterSurfaceMaterial::ParameterInfos;

    std::unordered_map<uint32_t, SurfaceMaterial::OptiXProgramSet> DirectionalEmitterSurfaceMaterial::s_optiXProgramSets;

    // static
    void DirectionalEmitterSurfaceMaterial::initialize(Context &context) {
        const ParameterInfo paramInfos[] = {
            ParameterInfo("emittance", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("scale", VLRParameterFormFlag_ImmediateValue, ParameterFloat),
            ParameterInfo("direction", VLRParameterFormFlag_Both, ParameterVector3D),
        };

        if (ParameterInfos.size() == 0) {
            ParameterInfos.resize(lengthof(paramInfos));
            std::copy_n(paramInfos, lengthof(paramInfos), ParameterInfos.data());
        }

        const char* identifiers[] = {
            RT_DC_NAME_STR("DirectionalEmitterSurfaceMaterial_setupEDF"),
            EDF_CALLABLE_NAMES("DirectionalEDF")
        };
        OptiXProgramSet programSet;
        commonInitializeProcedure(context, nullptr, identifiers, &programSet);

        s_optiXProgramSets[context.getID()] = programSet;
    }

    // static
    void DirectionalEmitterSurfaceMaterial::finalize(Context &context) {
        OptiXProgramSet &programSet = s_optiXProgramSets.at(context.getID());
        commonFinalizeProcedure(context, programSet);
        s_optiXProgramSets.erase(context.getID());
    }

    DirectionalEmitterSurfaceMaterial::DirectionalEmitterSurfaceMaterial(Context &context) :
        SurfaceMaterial(context),
        m_immEmittance(ColorSpace::Rec709_D65, M_PI, M_PI, M_PI), m_immScale(1.0f),
        m_immDirection(Vector3D(0, 0, 1)) {
        m_context.markSurfaceMaterialDescriptorDirty(this);
    }

    DirectionalEmitterSurfaceMaterial::~DirectionalEmitterSurfaceMaterial() {
    }

    void DirectionalEmitterSurfaceMaterial::setupMaterialDescriptor(CUstream stream) const {
        OptiXProgramSet &progSet = s_optiXProgramSets.at(m_context.getID());

        shared::SurfaceMaterialDescriptor matDesc;
        setupMaterialDescriptorHead(m_context, progSet, &matDesc);
        auto &mat = *matDesc.getData<shared::DirectionalEmitterSurfaceMaterial>();
        mat.nodeEmittance = m_nodeEmittance.getSharedType();
        mat.immEmittance = m_immEmittance.createTripletSpectrum(SpectrumType::LightSource);
        mat.immScale = m_immScale;
        mat.nodeDirection = m_nodeDirection.getSharedType();
        mat.immDirection = m_immDirection;

        m_context.updateSurfaceMaterialDescriptor(m_matIndex, matDesc, stream);
    }

    bool DirectionalEmitterSurfaceMaterial::get(const char* paramName, float* values, uint32_t length) const {
        if (values == nullptr)
            return false;

        if (testParamName(paramName, "scale")) {
            if (length != 1)
                return false;

            values[0] = m_immScale;
        }
        else {
            return false;
        }

        return true;
    }

    bool DirectionalEmitterSurfaceMaterial::get(const char* paramName, Vector3D* dir) const {
        if (dir == nullptr)
            return false;

        if (testParamName(paramName, "direction")) {
            *dir = m_immDirection;
        }
        else {
            return false;
        }

        return true;
    }

    bool DirectionalEmitterSurfaceMaterial::get(const char* paramName, ImmediateSpectrum* spectrum) const {
        if (spectrum == nullptr)
            return false;

        if (testParamName(paramName, "emittance")) {
            *spectrum = m_immEmittance;
        }
        else {
            return false;
        }

        return true;
    }

    bool DirectionalEmitterSurfaceMaterial::get(const char* paramName, ShaderNodePlug* plug) const {
        if (plug == nullptr)
            return false;

        if (testParamName(paramName, "emittance")) {
            *plug = m_nodeEmittance;
        }
        else if (testParamName(paramName, "direction")) {
            *plug = m_nodeDirection;
        }
        else {
            return false;
        }

        return true;
    }

    bool DirectionalEmitterSurfaceMaterial::set(const char* paramName, const float* values, uint32_t length) {
        if (testParamName(paramName, "scale")) {
            if (length != 1)
                return false;

            m_immScale = values[0];
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool DirectionalEmitterSurfaceMaterial::set(const char* paramName, const Vector3D& dir) {
        if (testParamName(paramName, "direction")) {
            m_immDirection = dir;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool DirectionalEmitterSurfaceMaterial::set(const char* paramName, const ImmediateSpectrum& spectrum) {
        if (testParamName(paramName, "emittance")) {
            m_immEmittance = spectrum;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool DirectionalEmitterSurfaceMaterial::set(const char* paramName, const ShaderNodePlug& plug) {
        if (testParamName(paramName, "emittance")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeEmittance = plug;
        }
        else if (testParamName(paramName, "direction")) {
            if (!shared::NodeTypeInfo<Vector3D>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeDirection = plug;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }



    std::vector<ParameterInfo> PointEmitterSurfaceMaterial::ParameterInfos;

    std::unordered_map<uint32_t, SurfaceMaterial::OptiXProgramSet> PointEmitterSurfaceMaterial::s_optiXProgramSets;

    // static
    void PointEmitterSurfaceMaterial::initialize(Context &context) {
        const ParameterInfo paramInfos[] = {
            ParameterInfo("intensity", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("scale", VLRParameterFormFlag_ImmediateValue, ParameterFloat),
        };

        if (ParameterInfos.size() == 0) {
            ParameterInfos.resize(lengthof(paramInfos));
            std::copy_n(paramInfos, lengthof(paramInfos), ParameterInfos.data());
        }

        const char* identifiers[] = {
            RT_DC_NAME_STR("PointEmitterSurfaceMaterial_setupEDF"),
            EDF_CALLABLE_NAMES("PointEDF")
        };
        OptiXProgramSet programSet;
        commonInitializeProcedure(context, nullptr, identifiers, &programSet);

        s_optiXProgramSets[context.getID()] = programSet;
    }

    // static
    void PointEmitterSurfaceMaterial::finalize(Context &context) {
        OptiXProgramSet &programSet = s_optiXProgramSets.at(context.getID());
        commonFinalizeProcedure(context, programSet);
        s_optiXProgramSets.erase(context.getID());
    }

    PointEmitterSurfaceMaterial::PointEmitterSurfaceMaterial(Context &context) :
        SurfaceMaterial(context), m_immIntensity(ColorSpace::Rec709_D65, M_PI, M_PI, M_PI), m_immScale(1.0f) {
        m_context.markSurfaceMaterialDescriptorDirty(this);
    }

    PointEmitterSurfaceMaterial::~PointEmitterSurfaceMaterial() {
    }

    void PointEmitterSurfaceMaterial::setupMaterialDescriptor(CUstream stream) const {
        OptiXProgramSet &progSet = s_optiXProgramSets.at(m_context.getID());

        shared::SurfaceMaterialDescriptor matDesc;
        setupMaterialDescriptorHead(m_context, progSet, &matDesc);
        auto &mat = *matDesc.getData<shared::PointEmitterSurfaceMaterial>();
        mat.nodeIntensity = m_nodeIntensity.getSharedType();
        mat.immIntensity = m_immIntensity.createTripletSpectrum(SpectrumType::LightSource);
        mat.immScale = m_immScale;

        m_context.updateSurfaceMaterialDescriptor(m_matIndex, matDesc, stream);
    }

    bool PointEmitterSurfaceMaterial::get(const char* paramName, float* values, uint32_t length) const {
        if (values == nullptr)
            return false;

        if (testParamName(paramName, "scale")) {
            if (length != 1)
                return false;

            values[0] = m_immScale;
        }
        else {
            return false;
        }

        return true;
    }

    bool PointEmitterSurfaceMaterial::get(const char* paramName, ImmediateSpectrum* spectrum) const {
        if (spectrum == nullptr)
            return false;

        if (testParamName(paramName, "intensity")) {
            *spectrum = m_immIntensity;
        }
        else {
            return false;
        }

        return true;
    }

    bool PointEmitterSurfaceMaterial::get(const char* paramName, ShaderNodePlug* plug) const {
        if (plug == nullptr)
            return false;

        if (testParamName(paramName, "intensity")) {
            *plug = m_nodeIntensity;
        }
        else {
            return false;
        }

        return true;
    }

    bool PointEmitterSurfaceMaterial::set(const char* paramName, const float* values, uint32_t length) {
        if (testParamName(paramName, "scale")) {
            if (length != 1)
                return false;

            m_immScale = values[0];
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool PointEmitterSurfaceMaterial::set(const char* paramName, const ImmediateSpectrum& spectrum) {
        if (testParamName(paramName, "intensity")) {
            m_immIntensity = spectrum;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool PointEmitterSurfaceMaterial::set(const char* paramName, const ShaderNodePlug& plug) {
        if (testParamName(paramName, "intensity")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeIntensity = plug;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }



    std::vector<ParameterInfo> MultiSurfaceMaterial::ParameterInfos;

    std::unordered_map<uint32_t, SurfaceMaterial::OptiXProgramSet> MultiSurfaceMaterial::s_optiXProgramSets;

    // static
    void MultiSurfaceMaterial::initialize(Context &context) {
        const ParameterInfo paramInfos[] = {
            ParameterInfo("0", VLRParameterFormFlag_Node, ParameterSurfaceMaterial),
            ParameterInfo("1", VLRParameterFormFlag_Node, ParameterSurfaceMaterial),
            ParameterInfo("2", VLRParameterFormFlag_Node, ParameterSurfaceMaterial),
            ParameterInfo("3", VLRParameterFormFlag_Node, ParameterSurfaceMaterial),
        };

        if (ParameterInfos.size() == 0) {
            ParameterInfos.resize(lengthof(paramInfos));
            std::copy_n(paramInfos, lengthof(paramInfos), ParameterInfos.data());
        }

        const char* bsdfIDs[] = {
            RT_DC_NAME_STR("MultiSurfaceMaterial_setupBSDF"),
            BSDF_CALLABLE_NAMES("MultiBSDF")
        };
        const char* edfIDs[] = {
            RT_DC_NAME_STR("MultiSurfaceMaterial_setupEDF"),
            EDF_CALLABLE_NAMES("MultiEDF")
        };
        OptiXProgramSet programSet;
        commonInitializeProcedure(context, bsdfIDs, edfIDs, &programSet);

        s_optiXProgramSets[context.getID()] = programSet;
    }

    // static
    void MultiSurfaceMaterial::finalize(Context &context) {
        OptiXProgramSet &programSet = s_optiXProgramSets.at(context.getID());
        commonFinalizeProcedure(context, programSet);
        s_optiXProgramSets.erase(context.getID());
    }

    MultiSurfaceMaterial::MultiSurfaceMaterial(Context &context) :
        SurfaceMaterial(context) {
        std::fill_n(m_subMaterials, lengthof(m_subMaterials), nullptr);
        m_context.markSurfaceMaterialDescriptorDirty(this);
    }

    MultiSurfaceMaterial::~MultiSurfaceMaterial() {
    }

    void MultiSurfaceMaterial::setupMaterialDescriptor(CUstream stream) const {
        OptiXProgramSet &progSet = s_optiXProgramSets.at(m_context.getID());

        shared::SurfaceMaterialDescriptor matDesc;
        setupMaterialDescriptorHead(m_context, progSet, &matDesc);
        auto &mat = *matDesc.getData<shared::MultiSurfaceMaterial>();

        mat.numSubMaterials = 0;
        std::fill_n(mat.subMatIndices, lengthof(mat.subMatIndices), 0xFFFFFFFF);
        for (int i = 0; i < lengthof(m_subMaterials); ++i) {
            if (m_subMaterials[i])
                mat.subMatIndices[mat.numSubMaterials++] = m_subMaterials[i]->getMaterialIndex();
        }

        m_context.updateSurfaceMaterialDescriptor(m_matIndex, matDesc, stream);
    }

    bool MultiSurfaceMaterial::get(const char* paramName, const SurfaceMaterial** material) const {
        if (material == nullptr)
            return false;

        if (strcmp(paramName, "0") == 0) {
            *material = m_subMaterials[0];
        }
        else if (strcmp(paramName, "1") == 0) {
            *material = m_subMaterials[1];
        }
        else if (strcmp(paramName, "2") == 0) {
            *material = m_subMaterials[2];
        }
        else if (strcmp(paramName, "3") == 0) {
            *material = m_subMaterials[3];
        }
        else {
            return false;
        }

        return true;
    }

    bool MultiSurfaceMaterial::set(const char* paramName, const SurfaceMaterial* material) {
        if (strcmp(paramName, "0") == 0) {
            m_subMaterials[0] = material;
        }
        else if (strcmp(paramName, "1") == 0) {
            m_subMaterials[1] = material;
        }
        else if (strcmp(paramName, "2") == 0) {
            m_subMaterials[2] = material;
        }
        else if (strcmp(paramName, "3") == 0) {
            m_subMaterials[3] = material;
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool MultiSurfaceMaterial::isEmitting() const {
        for (int i = 0; i < lengthof(m_subMaterials); ++i) {
            if (m_subMaterials[i]) {
                if (m_subMaterials[i]->isEmitting())
                    return true;
            }
        }
        return false;
    }



    std::vector<ParameterInfo> EnvironmentEmitterSurfaceMaterial::ParameterInfos;

    std::unordered_map<uint32_t, SurfaceMaterial::OptiXProgramSet> EnvironmentEmitterSurfaceMaterial::s_optiXProgramSets;

    // static
    void EnvironmentEmitterSurfaceMaterial::initialize(Context &context) {
        const ParameterInfo paramInfos[] = {
            ParameterInfo("emittance", VLRParameterFormFlag_Both, ParameterSpectrum),
            ParameterInfo("scale", VLRParameterFormFlag_ImmediateValue, ParameterFloat),
        };

        if (ParameterInfos.size() == 0) {
            ParameterInfos.resize(lengthof(paramInfos));
            std::copy_n(paramInfos, lengthof(paramInfos), ParameterInfos.data());
        }

        const char* identifiers[] = {
            RT_DC_NAME_STR("EnvironmentEmitterSurfaceMaterial_setupEDF"),
            EDF_CALLABLE_NAMES("EnvironmentEDF")
        };
        OptiXProgramSet programSet;
        commonInitializeProcedure(context, nullptr, identifiers, &programSet);

        s_optiXProgramSets[context.getID()] = programSet;
    }

    // static
    void EnvironmentEmitterSurfaceMaterial::finalize(Context &context) {
        OptiXProgramSet &programSet = s_optiXProgramSets.at(context.getID());
        commonFinalizeProcedure(context, programSet);
        s_optiXProgramSets.erase(context.getID());
    }

    EnvironmentEmitterSurfaceMaterial::EnvironmentEmitterSurfaceMaterial(Context &context) :
        SurfaceMaterial(context), m_immEmittance(ColorSpace::Rec709_D65, M_PI, M_PI, M_PI), m_immScale(1.0f) {
        m_context.markSurfaceMaterialDescriptorDirty(this);
    }

    EnvironmentEmitterSurfaceMaterial::~EnvironmentEmitterSurfaceMaterial() {
        m_importanceMap.finalize(m_context);
    }

    void EnvironmentEmitterSurfaceMaterial::setupMaterialDescriptor(CUstream stream) const {
        OptiXProgramSet &progSet = s_optiXProgramSets.at(m_context.getID());

        shared::SurfaceMaterialDescriptor matDesc;
        setupMaterialDescriptorHead(m_context, progSet, &matDesc);
        auto &mat = *matDesc.getData<shared::EnvironmentEmitterSurfaceMaterial>();
        mat.nodeEmittance = m_nodeEmittance.getSharedType();
        mat.immEmittance = m_immEmittance.createTripletSpectrum(SpectrumType::LightSource);
        mat.immScale = m_immScale;

        m_context.updateSurfaceMaterialDescriptor(m_matIndex, matDesc, stream);
    }

    bool EnvironmentEmitterSurfaceMaterial::get(const char* paramName, float* values, uint32_t length) const {
        if (values == nullptr)
            return false;

        if (testParamName(paramName, "scale")) {
            if (length != 1)
                return false;

            values[0] = m_immScale;
        }
        else {
            return false;
        }

        return true;
    }

    bool EnvironmentEmitterSurfaceMaterial::get(const char* paramName, ImmediateSpectrum* spectrum) const {
        if (spectrum == nullptr)
            return false;

        if (testParamName(paramName, "emittance")) {
            *spectrum = m_immEmittance;
        }
        else {
            return false;
        }

        return true;
    }

    bool EnvironmentEmitterSurfaceMaterial::get(const char* paramName, ShaderNodePlug* plug) const {
        if (plug == nullptr)
            return false;

        if (testParamName(paramName, "emittance")) {
            *plug = m_nodeEmittance;
        }
        else {
            return false;
        }

        return true;
    }

    bool EnvironmentEmitterSurfaceMaterial::set(const char* paramName, const float* values, uint32_t length) {
        if (testParamName(paramName, "scale")) {
            if (length != 1)
                return false;

            m_immScale = values[0];
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool EnvironmentEmitterSurfaceMaterial::set(const char* paramName, const ImmediateSpectrum& spectrum) {
        if (testParamName(paramName, "emittance")) {
            m_immEmittance = spectrum;
            if (m_importanceMap.isInitialized())
                m_importanceMap.finalize(m_context);
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    bool EnvironmentEmitterSurfaceMaterial::set(const char* paramName, const ShaderNodePlug& plug) {
        if (testParamName(paramName, "emittance")) {
            if (!shared::NodeTypeInfo<SampledSpectrum>::ConversionIsDefinedFrom(plug.getType()))
                return false;

            m_nodeEmittance = plug;
            if (m_importanceMap.isInitialized())
                m_importanceMap.finalize(m_context);
        }
        else {
            return false;
        }
        m_context.markSurfaceMaterialDescriptorDirty(this);

        return true;
    }

    const RegularConstantContinuousDistribution2D &EnvironmentEmitterSurfaceMaterial::getImportanceMap() {
        if (!m_importanceMap.isInitialized()) {
            if (m_nodeEmittance.node && m_nodeEmittance.node->is<EnvironmentTextureShaderNode>()) {
                auto node = reinterpret_cast<const EnvironmentTextureShaderNode*>(m_nodeEmittance.node);
                node->createImportanceMap(&m_importanceMap);
            }
            else {
                uint32_t mapWidth = 512;
                uint32_t mapHeight = 256;
                float* linearData = new float[mapWidth * mapHeight];
                std::fill_n(linearData, mapWidth * mapHeight, 1.0f);
                for (int y = 0; y < static_cast<int>(mapHeight); ++y) {
                    float theta = M_PI * (y + 0.5f) / mapHeight;
                    for (int x = 0; x < static_cast<int>(mapWidth); ++x) {
                        linearData[y * mapWidth + x] *= std::sin(theta);
                    }
                }

                m_importanceMap.initialize(m_context, linearData, mapWidth, mapHeight);

                delete[] linearData;
            }
        }

        return m_importanceMap;
    }

#undef EDF_CALLABLE_NAMES
#undef BSDF_CALLABLE_NAMES
}
