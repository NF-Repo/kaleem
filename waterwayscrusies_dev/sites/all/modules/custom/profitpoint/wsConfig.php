<?php
/**
 * Web-Tools - Configuration
 *
 * Common values for the PHP samples are stored here in order to keep them in
 * one place. A file such as this may be used in a production environment, however
 * the values would need to be changed.
 */

define('TRANSACTION_WSDL', 'https://api.profitpointinc.com:8417/v4/transaction?wsdl');
//REQUEST_ID has to be 1-6 character(s) long and contain only 0-Z(letters has to be capital). This constant is defined in wsForm.php.
//define('REQUEST_ID', '1'); 
define('LOCALE_ID', '');
define('SYSTEM_ID', 'PP');
define('CLIENT_ID', 'TEST');
define('LOCATION_ID', 'TEST001');
define('INTEGRATION_USER_NAME', 'Test_Auth');
define('INTEGRATION_PASSWORD', 'MoreRandMoreG00d');
define('TERMINAL_ID', 'API');
define('INITIATOR_TYPE', 'E');
define('INITIATOR_ID', 'Web');
define('INITIATOR_PASSWORD', '6789');
define('EXTERNAL_ID', ''); // Optional - reference to external order id?
define('BATCH_ID', '2010'); // Optional - increment dailiy if used
define('BATCH_REFERENCE', '1'); // Should increment for each transaction if used
define('CURRENCIES', 'US Dollars;USD;Canadian Dollars;CAD;Euro;EUR;Mexican Pesos;MXN;Japanese Yen;JPY');
define('CUSTOM_VALUES', 'Cups Of Coffee;COF');
