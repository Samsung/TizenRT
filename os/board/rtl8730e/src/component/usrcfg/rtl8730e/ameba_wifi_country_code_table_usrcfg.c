/*
* Copyright (c) 2024 Realtek Semiconductor Corp.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include "wifi_conf.h"

#define COUNTRY_CODE_LEN 2

struct country {
	char char2[COUNTRY_CODE_LEN];
};

struct country_code_table_common_t {
	const struct country *country_table;
	u8 channel_plan;
	u8 pwr_lmt;
	int num;
};

//Default table is modified according to Regulatory country list domain code-new format(internal) 20240104-R45.xlsx
const struct country_code_table_t country_code_table[] = {
	{{'A', 'R'}, 0x4d, TXPWR_LMT_FCC}, /* 0000 Argentina */
	{{'B', 'O'}, 0x11, TXPWR_LMT_FCC}, /* 0001 Bolivia */
	{{'B', 'R'}, 0x62, TXPWR_LMT_FCC}, /* 0002 Brazil */
	{{'C', 'L'}, 0x76, TXPWR_LMT_CHILE}, /* 0003 Chile */
	{{'M', 'X'}, 0x4d, TXPWR_LMT_MEXICO}, /* 0010 Mexico */
	{{'U', 'S'}, 0x1b, TXPWR_LMT_FCC}, /* 0015 USA */
	{{'U', 'Y'}, 0x30, TXPWR_LMT_FCC}, /* 0016 Uruguay */
	{{'V', 'E'}, 0x30, TXPWR_LMT_FCC}, /* 0017 Venezuela */
	{{'G', 'B'}, 0x0b, TXPWR_LMT_UK}, /* 0049 Great Britain (United Kingdom; England) */
	{{'B', 'H'}, 0x06, TXPWR_LMT_ETSI}, /* 0052 Bahrain */
	{{'E', 'G'}, 0x3c, TXPWR_LMT_ETSI}, /* 0056 Egypt */
	{{'I', 'Q'}, 0x05, TXPWR_LMT_ETSI}, /* 0058 Iraq */
	{{'M', 'A'}, 0x3c, TXPWR_LMT_ETSI}, /* 0068 Morocco */
	{{'N', 'G'}, 0x75, TXPWR_LMT_ETSI}, /* 0071 Nigeria */
	{{'Q', 'A'}, 0x5e, TXPWR_LMT_QATAR}, /* 0073 Qatar */
	{{'R', 'U'}, 0x09, TXPWR_LMT_ETSI}, /* 0075 Russian */
	{{'U', 'A'}, 0x5e, TXPWR_LMT_UKRAINE}, /* 0082 Ukraine */
	{{'C', 'N'}, 0x06, TXPWR_LMT_CN}, /* 0088 China */
	{{'I', 'D'}, 0x5d, TXPWR_LMT_ETSI}, /* 0091 Indonesia */
	{{'K', 'R'}, 0x4b, TXPWR_LMT_KCC}, /* 0092 South Korea */
	{{'M', 'Y'}, 0x07, TXPWR_LMT_ETSI}, /* 0093 Malaysia */
	{{'T', 'W'}, 0x76, TXPWR_LMT_NCC}, /* 0098 Taiwan */
	{{'A', 'U'}, 0x03, TXPWR_LMT_ACMA}, /* 0101 Australia */
	{{'N', 'Z'}, 0x03, TXPWR_LMT_ETSI}, /* 0102 New Zealand */
	{{'C', 'A'}, 0x10, TXPWR_LMT_IC}, /* 0104 Canada */
	{{'J', 'P'}, 0x7d, TXPWR_LMT_MKK}, /* 0105 Japan */
	{{'T', 'N'}, 0x04, TXPWR_LMT_ETSI}, /* 0109 Tunisia */
	{{'A', 'F'}, 0x42, TXPWR_LMT_ETSI}, /* 0110 Afghanistan */
	{{'D', 'Z'}, 0x00, TXPWR_LMT_ETSI}, /* 0111 Algeria */
	{{'A', 'Q'}, 0x26, TXPWR_LMT_ETSI}, /* 0116 Antarctica */
	{{'B', 'Y'}, 0x08, TXPWR_LMT_ETSI}, /* 0122 Belarus */
	{{'B', 'N'}, 0x06, TXPWR_LMT_ETSI}, /* 0131 Brunei */
	{{'C', 'X'}, 0x03, TXPWR_LMT_ACMA}, /* 0140 Christmas Island (Australia) */
	{{'C', 'C'}, 0x03, TXPWR_LMT_ETSI}, /* 0141 Cocos (Keeling) Islands (Australia) */
	{{'C', 'I'}, 0x42, TXPWR_LMT_ETSI}, /* 0146 Cote d'Ivoire */
	{{'H', 'M'}, 0x03, TXPWR_LMT_ACMA}, /* 0171 Heard and McDonald Islands (Australia) */
	{{'M', 'V'}, 0x3c, TXPWR_LMT_ETSI}, /* 0182 Maldives */
	{{'N', 'P'}, 0x06, TXPWR_LMT_ETSI}, /* 0194 Nepal */
	{{'N', 'U'}, 0x03, TXPWR_LMT_ACMA}, /* 0197 Niue */
	{{'N', 'F'}, 0x03, TXPWR_LMT_ACMA}, /* 0198 Norfolk Island (Australia) */
	{{'S', 'R'}, 0x74, TXPWR_LMT_FCC}, /* 0218 Suriname */
	{{'T', 'K'}, 0x03, TXPWR_LMT_ACMA}, /* 0224 Tokelau*/
	{{'T', 'V'}, 0x21, TXPWR_LMT_ETSI}, /* 0228 Tuvalu */
	{{'V', 'U'}, 0x26, TXPWR_LMT_ETSI}, /* 0232 Vanuatu */
	{{'E', 'H'}, 0x3c, TXPWR_LMT_ETSI}, /* 0234 Western Sahara */
	{{'0', '0'}, 0x17, TXPWR_LMT_WW}, /* 0238 World Wide */
};

/* These countries all use the FCC as the regulatory standard
Their channel_paln and pwr_lmt are set in country_code_table_common */
const struct country country_code_FCC[] = {
	{{'C', 'O'}}, /* 0004 Colombia */
	{{'C', 'R'}}, /* 0005 Costa Rica */
	{{'E', 'C'}}, /* 0006 Ecuador */
	{{'S', 'V'}}, /* 0007 El Salvador */
	{{'G', 'T'}}, /* 0008 Guatemala */
	{{'H', 'N'}}, /* 0009 Honduras*/
	{{'N', 'I'}}, /* 0011 Nicaragua */
	{{'P', 'A'}}, /* 0012 Panama */
	{{'P', 'Y'}}, /* 0013 Paraguay */
	{{'P', 'E'}}, /* 0014 Peru */
	{{'P', 'R'}}, /* 0018 Puerto Rico */
	{{'D', 'O'}}, /* 0019 Dominican Republic */
	{{'J', 'M'}}, /* 0106 Jamaica*/
	{{'A', 'N'}}, /* 0107 Netherlands Antilles */
	{{'T', 'T'}}, /* 0108 Trinidad & Tobago */
	{{'A', 'S'}}, /* 0112 American Samoa */
	{{'A', 'G'}}, /* 0117 Antigua & Barbuda */
	{{'A', 'W'}}, /* 0119 Aruba */
	{{'B', 'S'}}, /* 0120 Bahamas */
	{{'B', 'B'}}, /* 0121 Barbados */
	{{'B', 'Z'}}, /* 0123 Belize */
	{{'B', 'M'}}, /* 0125 Bermuda (UK) */
	{{'V', 'G'}}, /* 0130 British Virgin Islands (UK) */
	{{'K', 'Y'}}, /* 0137 Cayman Islands (UK) */
	{{'D', 'M'}}, /* 0148 Dominica */
	{{'F', 'J'}}, /* 0154 Fiji */
	{{'G', 'D'}}, /* 0163 Grenada */
	{{'G', 'U'}}, /* 0165 Guam (USA) */
	{{'G', 'Y'}}, /* 0169 Guyana */
	{{'H', 'T'}}, /* 0170 Haiti */
	{{'M', 'H'}}, /* 0184 Marshall Islands (USA) */
	{{'F', 'M'}}, /* 0189 Micronesia, Federated States of (USA) */
	{{'M', 'P'}}, /* 0199 Northern Mariana Islands (USA) */
	{{'P', 'W'}}, /* 0200 Palau */
	{{'K', 'N'}}, /* 0204 Saint Kitts and Nevis */
	{{'L', 'C'}}, /* 0205 Saint Lucia */
	{{'M', 'F'}}, /* 0206 Saint Martin */
	{{'S', 'X'}}, /* 0207 Sint Marteen */
	{{'V', 'C'}}, /* 0209 Saint Vincent and the Grenadines */
	{{'W', 'S'}}, /* 0210 Samoa */
	{{'S', 'C'}}, /* 0213 Seychelles */
	{{'V', 'I'}}, /* 0230 United States Virgin Islands (USA) */
};

/* These countries all use the ETSI as the regulatory standard
Their channel_paln and pwr_lmt are set in country_code_table_common */
const struct country country_code_ETSI[] = {
	{{'A', 'T'}}, /* 0020 Austria */
	{{'B', 'E'}}, /* 0021 Belgium */
	{{'C', 'Y'}}, /* 0022 Cyprus */
	{{'C', 'Z'}}, /* 0023 Czech Republic*/
	{{'D', 'K'}}, /* 0024 Denmark */
	{{'E', 'E'}}, /* 0025 Estonia */
	{{'F', 'I'}}, /* 0026 Finland */
	{{'F', 'R'}}, /* 0027 France */
	{{'D', 'E'}}, /* 0028 Germany */
	{{'G', 'R'}}, /* 0029 Greece */
	{{'H', 'U'}}, /* 0030 Hungary */
	{{'I', 'S'}}, /* 0031 Iceland */
	{{'I', 'E'}}, /* 0032 Ireland */
	{{'I', 'T'}}, /* 0033 Italy */
	{{'L', 'V'}}, /* 0034 Latvia */
	{{'L', 'I'}}, /* 0035 Liechtenstein */
	{{'L', 'T'}}, /* 0036 Lithuania */
	{{'L', 'U'}}, /* 0037 Luxembourg */
	{{'M', 'T'}}, /* 0038 Malta */
	{{'M', 'C'}}, /* 0039 Monaco */
	{{'N', 'L'}}, /* 0040 Netherlands */
	{{'N', 'O'}}, /* 0041 Norway */
	{{'P', 'L'}}, /* 0042 Poland */
	{{'P', 'T'}}, /* 0043 Portugal */
	{{'S', 'K'}}, /* 0044 Slovakia */
	{{'S', 'I'}}, /* 0045 Slovenia */
	{{'E', 'S'}}, /* 0046 Spain */
	{{'S', 'E'}}, /* 0047 Sweden */
	{{'C', 'H'}}, /* 0048 Switzerland */
	{{'A', 'L'}}, /* 0050 Albania */
	{{'A', 'Z'}}, /* 0051 Azerbaijan */
	{{'B', 'A'}}, /* 0053 Bosnia & Herzegovina */
	{{'B', 'G'}}, /* 0054 Bulgaria */
	{{'H', 'R'}}, /* 0055 Croatia */
	{{'G', 'H'}}, /* 0057 Ghana */
	{{'I', 'L'}}, /* 0059 Israel */
	{{'J', 'O'}}, /* 0060 Jordan */
	{{'K', 'Z'}}, /* 0061 Kazakhstan */
	{{'K', 'E'}}, /* 0062 Kenya */
	{{'K', 'W'}}, /* 0063 Kuwait */
	{{'K', 'G'}}, /* 0064 Kyrgyzstan */
	{{'L', 'B'}}, /* 0065 Lebanon */
	{{'L', 'S'}}, /* 0066 Lesotho */
	{{'M', 'K'}}, /* 0067 Republic of Macedonia (FYROM) */
	{{'M', 'Z'}}, /* 0069 Mozambique */
	{{'N', 'A'}}, /* 0070 Namibia */
	{{'O', 'M'}}, /* 0072 Oman */
	{{'R', 'O'}}, /* 0074 Romania */
	{{'S', 'A'}}, /* 0076 Saudi Arabia */
	{{'R', 'S'}}, /* 0078 Serbia */
	{{'Z', 'A'}}, /* 0080 South Africa */
	{{'T', 'R'}}, /* 0081 Turkey */
	{{'A', 'E'}}, /* 0083 United Arab Emirates */
	{{'Z', 'W'}}, /* 0085 Zimbabwe */
	{{'B', 'D'}}, /* 0086 Bangladesh */
	{{'K', 'H'}}, /* 0087 Cambodia */
	{{'H', 'K'}}, /* 0089 Hong Kong */
	{{'I', 'N'}}, /* 0090 India */
	{{'P', 'K'}}, /* 0094 Pakistan */
	{{'P', 'H'}}, /* 0095 Philippines */
	{{'S', 'G'}}, /* 0096 Singapore */
	{{'L', 'K'}}, /* 0097 Sri Lanka */
	{{'T', 'H'}}, /* 0099 Thailand */
	{{'V', 'N'}}, /* 0100 Vietnam */
	{{'P', 'G'}}, /* 0103 Papua New Guinea */
	{{'A', 'O'}}, /* 0114 Angola */
	{{'A', 'I'}}, /* 0115 Anguilla(UK) */
	{{'A', 'M'}}, /* 0118 Armenia */
	{{'B', 'T'}}, /* 0126 Bhutan */
	{{'B', 'W'}}, /* 0127 Botswana */
	{{'B', 'V'}}, /* 0128 Bouvet Island (Norway) */
	{{'B', 'F'}}, /* 0132 Burkina Faso */
	{{'C', 'M'}}, /* 0135 Cameroon */
	{{'C', 'V'}}, /* 0136 Cape Verde */
	{{'K', 'M'}}, /* 0142 Comoros */
	{{'C', 'G'}}, /* 0143 Congo, Democratic Republic of the. Zaire */
	{{'C', 'D'}}, /* 0144 Congo, Republic of the */
	{{'C', 'K'}}, /* 0145 Cook Islands */
	{{'D', 'J'}}, /* 0147 Djibouti */
	{{'G', 'Q'}}, /* 0149 Equatorial Guinea */
	{{'F', 'K'}}, /* 0152 Falkland Islands (Islas Malvinas) (UK) */
	{{'F', 'O'}}, /* 0153 Faroe Islands (Denmark) */
	{{'G', 'A'}}, /* 0158 Gabon */
	{{'G', 'M'}}, /* 0159 Gambia */
	{{'G', 'E'}}, /* 0160 Georgia */
	{{'G', 'I'}}, /* 0161 Gibraltar (UK) */
	{{'G', 'L'}}, /* 0162 Greenland (Denmark) */
	{{'G', 'P'}}, /* 0164 Guadeloupe (France) */
	{{'G', 'G'}}, /* 0166 Guernsey (UK)*/
	{{'G', 'N'}}, /* 0167 Guinea */
	{{'G', 'W'}}, /* 0168 Guinea-Bissau */
	{{'V', 'A'}}, /* 0172 Holy See (Vatican City) */
	{{'I', 'M'}}, /* 0173 Isle of Man (UK) */
	{{'J', 'E'}}, /* 0174 Jersey (UK) */
	{{'K', 'I'}}, /* 0175 Kiribati */
	{{'L', 'A'}}, /* 0176 Laos */
	{{'L', 'R'}}, /* 0177 Liberia */
	{{'L', 'Y'}}, /* 0178 Libya */
	{{'M', 'O'}}, /* 0179 Macau */
	{{'M', 'G'}}, /* 0180 Madagascar */
	{{'M', 'W'}}, /* 0181 Malawi */
	{{'M', 'L'}}, /* 0183 Mali */
	{{'M', 'R'}}, /* 0186 Mauritania */
	{{'M', 'U'}}, /* 0187 Mauritius */
	{{'Y', 'T'}}, /* 0188 Mayotte (France) */
	{{'M', 'D'}}, /* 0190 Moldova */
	{{'M', 'N'}}, /* 0191 Mongolia */
	{{'N', 'R'}}, /* 0193 Nauru */
	{{'N', 'E'}}, /* 0196 Niger */
	{{'R', 'E'}}, /* 0201 Reunion (France) */
	{{'R', 'W'}}, /* 0202 Rwanda */
	{{'S', 'H'}}, /* 0203 Saint Helena (UK) */
	{{'P', 'M'}}, /* 0208 Saint Pierre and Miquelon (France) */
	{{'S', 'T'}}, /* 0212 Sao Tome and Principe */
	{{'S', 'L'}}, /* 0214 Sierra Leone */
	{{'S', 'J'}}, /* 0219 Svalbard (Norway)*/
	{{'S', 'Z'}}, /* 0220 Swaziland */
	{{'T', 'J'}}, /* 0221 Tajikistan */
	{{'T', 'Z'}}, /* 0222 Tanzania */
	{{'T', 'G'}}, /* 0223 Togo */
	{{'Z', 'M'}}, /* 0235 Zambia */
	{{'P', 'S'}}, /* 0237 Palestine*/
};

/* These countries all use the ETSI as the regulatory standard
Their channel_paln and pwr_lmt are set in country_code_table_common */
const struct country country_code_ETSI2[] = {
	{{'S', 'N'}}, /* 0077 Senegal */
	{{'M', 'E'}}, /* 0079 Montenegro */
	{{'Y', 'E'}}, /* 0084 Yemen */
	{{'A', 'D'}}, /* 0113 Andorra */
	{{'B', 'J'}}, /* 0124 Benin */
	{{'I', 'O'}}, /* 0129 British Indian Ocean Territory (UK) */
	{{'M', 'M'}}, /* 0133 Burma (Myanmar) */
	{{'B', 'I'}}, /* 0134 Burundi */
	{{'C', 'F'}}, /* 0138 Central African Republic */
	{{'T', 'D'}}, /* 0139 Chad */
	{{'E', 'R'}}, /* 0150 Eritrea */
	{{'E', 'T'}}, /* 0151 Ethiopia */
	{{'G', 'F'}}, /* 0155 French Guiana */
	{{'P', 'F'}}, /* 0156 French Polynesia (France) */
	{{'T', 'F'}}, /* 0157 French Southern and Antarctic Lands (FR Southern Territories) */
	{{'M', 'Q'}}, /* 0185 Martinique (France) */
	{{'M', 'S'}}, /* 0192 Montserrat (UK) */
	{{'N', 'C'}}, /* 0195 New Caledonia */
	{{'S', 'M'}}, /* 0211 San Marino */
	{{'S', 'B'}}, /* 0215 Solomon Islands */
	{{'S', 'O'}}, /* 0216 Somalia */
	{{'G', 'S'}}, /* 0217 South Georgia and the Sandwich Islands (UK) */
	{{'T', 'O'}}, /* 0225 Tonga */
	{{'T', 'M'}}, /* 0226 Turkmenistan */
	{{'T', 'C'}}, /* 0227 Turks and Caicos Islands (UK) */
	{{'U', 'G'}}, /* 0229 Uganda */
	{{'U', 'Z'}}, /* 0231 Uzbekistan */
	{{'W', 'F'}}, /* 0233 Wallis and Futuna (France) */
	{{'I', 'R'}}, /* 0236 Iran */
};

const struct country_code_table_common_t country_code_table_common[] = {
	/*Most country have the same power limit and channel plan*/
	{country_code_FCC, 0x76, TXPWR_LMT_FCC, sizeof(country_code_FCC) / sizeof(country_code_FCC[0])},
	{country_code_ETSI, 0x5e, TXPWR_LMT_ETSI, sizeof(country_code_ETSI) / sizeof(country_code_ETSI[0])},
	{country_code_ETSI2, 0x3a, TXPWR_LMT_ETSI, sizeof(country_code_ETSI2) / sizeof(country_code_ETSI2[0])},
};

BOOLEAN wifi_get_country_code_info(char *country_code, u8 *channel_plan, u8 *power_limit)
{
	u8 i, j;
	if (!country_code) {
		return _FALSE;
	}

	for (i = 0; i < sizeof(country_code_table) / sizeof(country_code_table[0]); i++) {
		if (country_code_table[i].char2[0] == country_code[0] && country_code_table[i].char2[1] == country_code[1]) {
			if (channel_plan) {
				*channel_plan = country_code_table[i].channel_plan;
			}

			if (power_limit) {
				*power_limit = country_code_table[i].pwr_lmt;
			}
			return _TRUE;
		}
	}

	for (i = 0; i < sizeof(country_code_table_common) / sizeof(country_code_table_common[0]); i++) {
		for (j = 0; j < country_code_table_common[i].num; j++) {
			if (country_code_table_common[i].country_table[j].char2[0] == country_code[0] && country_code_table_common[i].country_table[j].char2[1] == country_code[1]) {
				if (channel_plan) {
					*channel_plan = country_code_table_common[i].channel_plan;
				}

				if (power_limit) {
					*power_limit = country_code_table_common[i].pwr_lmt;
				}
				return _TRUE;
			}
		}
	}

	return _FALSE;
}
