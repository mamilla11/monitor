#include "Gsm.h"

namespace net {

namespace base {

namespace sim800 {

bool Gsm::set_connection_type(ConnectionType type)
{
	char at[Sim800Base::MAX_AT_LENGTH];

	switch (type)
	{
	case ConnectionType::CSD:
		snprintf(at, MAX_AT_LENGTH, "AT+SAPBR=3,1,\"CONTYPE\",\"CSD\"\r\n"); break;
	case ConnectionType::GPRS:
		snprintf(at, MAX_AT_LENGTH, "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n"); break;
	default: return false;
	}

	send_raw((const char*)at);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS)
		return false;

	return true;
}

bool Gsm::set_apn(const char *apn)
{
	char at[MAX_AT_LENGTH];

	snprintf(at, MAX_AT_LENGTH, "AT+SAPBR=3,1,\"APN\",\"%s\"\r\n", apn);
	send_raw((const char*)at);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS)
		return false;

	return true;
}

bool Gsm::set_user(const char *user)
{
	char at[MAX_AT_LENGTH];

	snprintf(at, MAX_AT_LENGTH, "AT+SAPBR=3,1,\"USER\",\"%s\"\r\n", user);
	send_raw((const char*)at);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS)
		return false;

	return true;
}

bool Gsm::set_pwd(const char *pwd)
{
	char at[MAX_AT_LENGTH];

	snprintf(at, MAX_AT_LENGTH, "AT+SAPBR=3,1,\"PWD\",\"%s\"\r\n", pwd);
	send_raw((const char*)at);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS)
		return false;

	return true;
}

bool Gsm::open_bearer()
{
	send_raw("AT+SAPBR=1,1\r\n");

	if (recv_status(TIMEOUT_30S+TIMEOUT_60S) != CommandStatus::SUCCESS)
		return false;

	return true;
}

bool Gsm::query_bearer()
{
	send_raw("AT+SAPBR=2,1\r\n");

	if (recv_string(TIMEOUT_10S, "+SAPBR: 1,1,") != CommandStatus::SUCCESS) {
		return false;
	}

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS)
		return false;

	return true;
}

bool Gsm::close_bearer()
{
	send_raw("AT+SAPBR=0,1\r\n");

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS)
		return false;

	return true;
}

bool Gsm::set_remote_delay(uint16_t rd_single, uint16_t rd_multi)
{
	char at[MAX_AT_LENGTH];

	snprintf(at, MAX_AT_LENGTH, "AT+CIPRDTIMER=%d,%d\r\n", rd_single, rd_multi);
	send_raw((const char*)at);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS)
		return false;

	return true;
}

bool Gsm::set_multi_ip_connection()
{
	send_raw("AT+CIPMUX=1\r\n");

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS)
		return false;

	return true;
}

bool Gsm::get_data_from_network_manually()
{
	send_raw("AT+CIPRXGET=1\r\n");

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS)
		return false;

	return true;
}

bool Gsm::start_task_n_set_apn(const char *apn)
{
	char at[MAX_AT_LENGTH];

	snprintf(at, MAX_AT_LENGTH, "AT+CSTT=\"%s\"\r\n", apn);

	send_raw((const char*)at);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS)
		return false;

	send_raw("AT+CSTT?\r\n");
	recv_status(TIMEOUT_10S);

	return true;
}

bool Gsm::wait_net_registration()
{
	for (int i = 0; i < 10; i++)
	{
		delay(TIMEOUT_1S);

		send_raw("AT+CGREG?\r\n");

		if (recv_string(TIMEOUT_10S, "+CGREG: ") != CommandStatus::SUCCESS)
			continue;

		uint32_t status = atoi(&received_string.data()[strlen("+CGREG: , ")]);

		recv_status(TIMEOUT_5S);

		if (status == 2)
			continue;

		if ((status == 1) || (status == 5))
			return true;
	}

	return false;
}

bool Gsm::wait_gprs_attached()
{
	send_raw("AT+CGATT?\r\n");

	if (recv_string(TIMEOUT_10S, "+CGATT: 1") != CommandStatus::SUCCESS)
		return false;

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS)
		return false;
	return true;
}

bool Gsm::shut_tcp()
{
	send_raw("AT+CIPSHUT\r\n");
	if (recv_string(TIMEOUT_60S, "SHUT OK") != CommandStatus::SUCCESS)
		return false;
	return(true);

}

bool Gsm::reset_sim()
{
	send_raw("AT+CFUN=1,1\r\n");
	if (recv_status(TIMEOUT_3S) != CommandStatus::SUCCESS)
		return false;
	return(true);
}

bool Gsm::set_wireless_connection()
{
	send_raw("AT+CIICR\r\n");

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS)
		return false;
	return true;
}

void Gsm::get_local_ip_address(char *buffer)
{
	send_raw("AT+CIFSR\r\n");

	if (recv_string(TIMEOUT_60S, "ANY") != CommandStatus::SUCCESS)
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
	else
		std::copy(received_string.begin(), received_string.end(), buffer);
}

void Gsm::get_manufacturer(char *buffer)
{
	send_raw("AT+CGMI\r\n");

	if (recv_string(TIMEOUT_10S, "ANY") != CommandStatus::SUCCESS)
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
	else
		std::copy(received_string.begin(), received_string.end(), buffer);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}
}

void Gsm::get_model(char *buffer)
{
	send_raw("AT+CGMM\r\n");

	if (recv_string(TIMEOUT_10S, "ANY") != CommandStatus::SUCCESS)
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
	else
		std::copy(received_string.begin(), received_string.end(), buffer);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}
}

void Gsm::get_version(char *buffer)
{
	send_raw("AT+GMR\r\n");

	if (recv_string(TIMEOUT_10S, "ANY") != CommandStatus::SUCCESS)
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
	else
		std::copy(received_string.begin(), received_string.end(), buffer);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}
}

void Gsm::get_IMEI(char *buffer)
{
	send_raw("AT+CGSN\r\n");

	if (recv_string(TIMEOUT_10S, "ANY") != CommandStatus::SUCCESS)
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
	else
		std::copy(received_string.begin(), received_string.end(), buffer);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}
}

void Gsm::get_IMSI(char *buffer)
{
	send_raw("AT+CIMI\r\n");

	if (recv_string(TIMEOUT_10S, "ANY") != CommandStatus::SUCCESS)
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
	else
		std::copy(received_string.begin(), received_string.end(), buffer);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}
}

void Gsm::get_ICCID(char *buffer)
{
	send_raw("AT+CCID\r\n");

	if (recv_string(TIMEOUT_10S, "ANY") != CommandStatus::SUCCESS)
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
	else
		std::copy(received_string.begin(), received_string.end(), buffer);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}
}

void Gsm::get_oper_name(char *buffer)
{
	send_raw("AT+COPS=0,0\r\n");

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}

	send_raw("AT+COPS?\r\n");

	if (recv_string(TIMEOUT_10S, "+COPS: 0,0,") != CommandStatus::SUCCESS)
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
	else
		snprintf(buffer, utils::config::MAX_INF_STRING_LEN, &received_string.data()[strlen("+COPS: 0,0,")]);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}
}

void Gsm::get_oper_code(char *buffer)
{
	send_raw("AT+COPS=0,2\r\n");

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}

	send_raw("AT+COPS?\r\n");

	if (recv_string(TIMEOUT_10S, "+COPS: 0,2,") != CommandStatus::SUCCESS)
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
	else
		snprintf(buffer, utils::config::MAX_INF_STRING_LEN, &received_string.data()[strlen("+COPS: 0,2,")]);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}
}

void Gsm::get_LAC_n_ID(char *buffer)
{
	send_raw("AT+CREG=2\r\n");

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}

	send_raw("AT+CREG?\r\n");

	if (recv_string(TIMEOUT_10S, "+CREG: 2,1,") != CommandStatus::SUCCESS)
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
	else
		snprintf(buffer, utils::config::MAX_INF_STRING_LEN, &received_string.data()[strlen("+CREG: 2,1,")]);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}
}

void Gsm::get_location(char *buffer)
{
	send_raw("AT+CIPGSMLOC=1,1\r\n");

	if (recv_string(TIMEOUT_60S, "+CIPGSMLOC: 0,") != CommandStatus::SUCCESS)
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
	else {
		for (int i = 0; i <= 20; i++)
			received_string.pop_back();
		snprintf(buffer, utils::config::MAX_INF_STRING_LEN, &received_string.data()[strlen("+CIPGSMLOC: 0,")]);
	}

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}
}

void Gsm::get_time(char *buffer)
{
	send_raw("AT+CIPGSMLOC=2,1\r\n");

	if (recv_string(TIMEOUT_10S, "+CIPGSMLOC: 0,") != CommandStatus::SUCCESS)
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
	else
		snprintf(buffer, utils::config::MAX_INF_STRING_LEN, &received_string.data()[strlen("+CIPGSMLOC: 0,")]);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}
}

void Gsm::get_signal_quality(char *buffer)
{
	send_raw("AT+CSQ\r\n");

	if (recv_string(TIMEOUT_10S, "+CSQ: ") != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}

	uint32_t rssi = atoi(&received_string.data()[strlen("+CSQ: ")]);

	if(rssi == 99)
		snprintf(buffer, utils::config::MAX_INF_STRING_LEN, "0%%");
	else if(rssi < 8)
		snprintf(buffer, utils::config::MAX_INF_STRING_LEN, "25%%");
	else if((rssi >= 8) && (rssi < 16))
		snprintf(buffer, utils::config::MAX_INF_STRING_LEN, "50%%");
	else if((rssi >= 16) && (rssi < 24))
		snprintf(buffer, utils::config::MAX_INF_STRING_LEN, "75%%");
	else if((rssi >= 24) && (rssi < 32))
		snprintf(buffer, utils::config::MAX_INF_STRING_LEN, "100%%");

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return;
	}
}

bool Gsm::ip_from_dns(char *dns, char *buffer)
{
	char at[MAX_AT_LENGTH];

	snprintf(at, MAX_AT_LENGTH, "AT+CDNSGIP=%s\r\n", dns);
	send_raw((const char*)at);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS) {
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
		return false;
	}

	if (recv_string(TIMEOUT_60S, "+CDNSGIP: 1,") != CommandStatus::SUCCESS)
		std::copy(_NULL_STR.begin(), _NULL_STR.end(), buffer);
	else
		snprintf(buffer, utils::config::MAX_INF_STRING_LEN, &received_string.data()[strlen("+CDNSGIP: 1,") + strlen(dns) + 4]);

	return true;
}

bool Gsm::define_pdp_context(uint8_t cid, const char* type, const char* apn)
{
	char at[MAX_AT_LENGTH];

	snprintf(at, MAX_AT_LENGTH, "AT+CGDCONT=%d,\"%s\",\"%s\"\r\n", cid, type, apn);
	send_raw((const char*)at);

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS)
		return false;

	return true;
}

bool Gsm::activate_pdp_context()
{
	send_raw("AT+CGACT=1\r\n");

	if (recv_status(TIMEOUT_3S) != CommandStatus::SUCCESS)
		return false;

	send_raw("AT+CGACT?\r\n");

	if (recv_string(TIMEOUT_10S, "+CGACT:") != CommandStatus::SUCCESS)
		return false;

	if (recv_status(TIMEOUT_3S) != CommandStatus::SUCCESS)
		return false;

	return(true);
}

bool Gsm::test()
{
	send_raw("ATD*99***1#\r\n");

	if (recv_string(TIMEOUT_10S, "ANY") != CommandStatus::SUCCESS)
		return false;

	return true;
}

bool Gsm::enter_data_state(const char* l2p, uint8_t cid)
{
	char at[MAX_AT_LENGTH];

	snprintf(at, MAX_AT_LENGTH, "AT+CGDATA=\"%s\",%d\r\n", l2p, cid);
	send_raw((const char*)at);

	if (recv_string(TIMEOUT_10S, "CONNECT") != CommandStatus::SUCCESS)
		return false;

	return true;
}

} /* namespace sim800 */

} /* namespace base */

} /* namespace net */
