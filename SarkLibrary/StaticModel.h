#ifndef __STATIC_MODEL_H__
#define __STATIC_MODEL_H__

#include "AModel.h"

namespace sark{

	// static mesh model.
	class StaticModel : public AModel{
	public:
		StaticModel(const std::string& name, ASceneComponent* parent, bool activate);

		virtual ~StaticModel();

		virtual void Update() override;

		virtual void Render() override;
	};

}
#endif