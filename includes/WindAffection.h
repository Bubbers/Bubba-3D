#pragma once


class WindAffection {
public:
    bool getIsAffectedByWind() const;
    void setAffectedByWind(bool isAffectedByWind);

    float getMainBendiness() const;
    void setMainBendiness(float mainBendiness);

    float getBranchAmplitude() const;
    void setBranchAmplitude(float branchAmplitude);

    float getLeafAmplitude() const;
    void setLeafAmplitude(float leafAmplitude);

private:
    bool isAffectedByWind = false;
    float mainBendiness = 0.0f;
    float branchAmplitude = 0.0f;
    float leafAmplitude = 0.0f;
};



