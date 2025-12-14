#pragma once

#include "OverlayMenuSimComp.hpp"

namespace gui
{
	namespace gl
	{
		class OverlayMenuSimComp : public AOverlayMenuSimComp
		{
		public:
			OverlayMenuSimComp(ps::ProgramState& program_state, Dispatcher& dispathcer);
			virtual ~OverlayMenuSimComp();

			// Inherited via AGUIMenu
			virtual void topHeaderInfo() override;
			virtual void submenuForcefinderOption() override;
			virtual void submenuIntegratorOption() override;
			virtual void submenuG() override;
			virtual void submenuSoft() override;
			virtual void submenuStepsize() override;
			virtual void submenuBodiesCount() override;
			virtual void submenuResetBodies() override;
			virtual void submenuShapeSize() override;
			virtual void submenuMiscellaneous() override;
			
		private:

			bool _isNumberBodiesHovered = false;

		private:

			bool _isForcefinderValid();
			bool _isIntegratorValid();
			bool _isNumberBodiesValid();

		};
	}
}

