#pragma once

#include "OverlayMenuSimComp.hpp"


namespace gui
{
	namespace host
	{
		class OverlayMenuSimComp : public AOverlayMenuSimComp
		{
		public:
			OverlayMenuSimComp(ps::ProgramState& program_state, Dispatcher& dispathcer);
			virtual ~OverlayMenuSimComp() {}

			// Inherited via AOverlayMenuSimComp
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

		protected:

			int workers_count = 0;

		private:

			bool _isForcefinderValid();
			bool _isIntegratorValid();

		};
	}
}
