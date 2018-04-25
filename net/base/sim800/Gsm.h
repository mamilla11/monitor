#pragma once

#include "Sim800Base.h"

namespace net {

namespace base {

namespace sim800 {

class Gsm : public Sim800Base
{
public:
	enum class ConnectionType
	{
		CSD,
		GPRS,
	};

	Gsm()  = default;
	~Gsm() = default;
    /**
     * @brief Sets up connection type
     *
     * @param type CSD or GPRS
     * @retval true if command executed, false otherwise
     */
	bool set_connection_type(ConnectionType type);
    /**
     * @brief Sets up APN
     *
     * @param apn access point name
     * @retval true if command executed, false otherwise
     */
	bool set_apn(const char *apn);
    /**
     * @brief Sets up USERNAME
     *
     * @param user username
     * @retval true if command executed, false otherwise
     */
	bool set_user(const char *user);
    /**
     * @brief Sets up password
     *
     * @param pwd password
     * @retval true if command executed, false otherwise
     */
	bool set_pwd(const char *pwd);
    /**
     * @brief Opens bearer
     *
     * @retval true if command executed, false otherwise
     */
	bool open_bearer();
    /**
     * @brief Queries bearer
     *
     * @retval true if command executed, false otherwise
     */
	bool query_bearer();
    /**
     * @brief Closes bearer
     *
     * @retval true if command executed, false otherwise
     */
	bool close_bearer();
    /**
     * @brief Sets up remote delay timer
     *
     * @param rd_single remote delay for single connection
     * @param rd_multi remote delay for multi connection
     * @retval true if command executed, false otherwise
     */
	bool set_remote_delay(uint16_t rd_single, uint16_t rd_multi);
    /**
     * @brief Starts up Multi-IP connection
     *
     * @retval true if command executed, false otherwise
     */
	bool set_multi_ip_connection();
    /**
     * @brief Configures TCP to get data from network manually
     *
     * @retval true if command executed, false otherwise
     */
	bool get_data_from_network_manually();
    /**
     * @brief Starts task and sets up APN
     *
     * @param apn access point name
     * @retval true if command executed, false otherwise
     */
	bool start_task_n_set_apn(const char *apn);
    /**
     * @brief Checks if GPRS service is attached
     *
     * @retval true if command executed, false otherwise
     */
	bool wait_gprs_attached();
    /**
     * @brief Waits for registration in a network
     *
     * @retval true if command executed, false otherwise
     */
	bool wait_net_registration();
    /**
     * @brief Releases all resources of closed TCP connections if any
     *
     * @retval true if command executed, false otherwise
     */
	bool shut_tcp();
    /**
     * @brief Resets SIM card and enables full functionality
     *
     * @retval true if command executed, false otherwise
     */
	bool reset_sim();
    /**
     * @brief Sets up wireless connection
     *
     * @retval true if command executed, false otherwise
     */
	bool set_wireless_connection();
    /**
     * @brief Gets local IP address
     *
     * @param buffer to store local IP address
     */
	void get_local_ip_address(char *buffer);
    /**
     * @brief Gets manufacturer
     *
     * @param buffer to store manufacturer
     */
	void get_manufacturer(char *buffer);
    /**
     * @brief Gets model
     *
     * @param buffer to store model
     */
	void get_model(char *buffer);
    /**
     * @brief Gets version
     *
     * @param buffer to store version
     */
	void get_version(char *buffer);
    /**
     * @brief Gets IMEI
     *
     * @param buffer to store IMEI
     */
	void get_IMEI(char *buffer);
    /**
     * @brief Gets IMSI
     *
     * @param buffer to store IMSI
     */
	void get_IMSI(char *buffer);
    /**
     * @brief Gets ICCID
     *
     * @param buffer to store ICCID
     */
	void get_ICCID(char *buffer);
    /**
     * @brief Gets operator name
     *
     * @param buffer to store operator name
     */
	void get_oper_name(char *buffer);
    /**
     * @brief Gets operator code
     *
     * @param buffer to store operator code
     */
	void get_oper_code(char *buffer);
    /**
     * @brief Gets LAC and ID
     *
     * @param buffer to store LAC and ID
     */
	void get_LAC_n_ID(char *buffer);
    /**
     * @brief Gets location and time
     *
     * @param buffer to store location and time
     */
	void get_location(char *buffer);
    /**
     * @brief Gets time
     *
     * @param buffer to store time
     */
	void get_time(char *buffer);
    /**
     * @brief Gets signal quality
     *
     * @param buffer to store signal quality
     */
	void get_signal_quality(char *buffer);
    /**
     * @brief Gets IP from DNS
     *
     * @param dns domain name (DNS)
     * @param buffer to store IP
     * @retval true if command executed, false otherwise
     */
    bool ip_from_dns(char *dns, char *buffer);

    bool define_pdp_context(uint8_t cid, const char* type, const char* apn);

    bool activate_pdp_context();

    bool enter_data_state(const char* l2p, uint8_t cid);

private:
    SmallString _NULL_STR = "NULL";
};

} /* namespace sim800 */

} /* namespace base */

} /* namespace net */
