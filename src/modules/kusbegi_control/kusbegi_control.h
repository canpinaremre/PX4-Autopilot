#pragma once

#include <drivers/drv_hrt.h>
#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>

#include <uORB/topics/position_setpoint_triplet.h>
#include <uORB/topics/kusbegi_mission.h>
#include <uORB/topics/kusbegi_target.h>

#include <uORB/Publication.hpp>
#include <uORB/Subscription.hpp>


class KusbegiControl : public ModuleBase<KusbegiControl>, public ModuleParams, public px4::ScheduledWorkItem
{
public:
	KusbegiControl();
	~KusbegiControl() override;

	bool get_mcu_message();
	bool send_message_to_mcu();

	static int task_spawn(int argc, char *argv[]);

	/** @see ModuleBase */
	static int custom_command(int argc, char *argv[])
	{
		return print_usage("unknown command");
	}

	/** @see ModuleBase */
	static int print_usage(const char *reason = nullptr);

	void start();
	int print_status() override;
private:

	/** Do a compute and schedule the next cycle. */
	void Run() override;

	kusbegi_mission_s	_kusbegi_mission_s{};
	kusbegi_target_s	_kusbegi_target_s{};

	kusbegi_mission_s	_cmd_mission_s{};


	uORB::Publication<kusbegi_mission_s>			_kusbegi_mission_pub{ORB_ID(kusbegi_mission)};
	uORB::Publication<kusbegi_target_s>			_kusbegi_target_pub{ORB_ID(kusbegi_target)};

	uORB::Subscription					_kusbegi_target_sub{ORB_ID(kusbegi_target)};
	uORB::Subscription					_kusbegi_mission_sub{ORB_ID(kusbegi_mission)};

	enum flightPhase {
		IDLE = 0,
		TAKEOFF = 1,
		TO_WP = 2,
		LAND = 3,
		FLIGHT_TASK = 4,
		FAIL_SAFE = 5
	};

	enum commandResult{
		CMD_OK,
		CMD_ERROR,
		NO_CMD
	};
	commandResult _cmd_result{NO_CMD};
	flightPhase _phase{flightPhase::IDLE};
	int mytest{0};

};