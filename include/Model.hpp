#ifndef _MODEL
#define _MODEL

extern "C"
{
    constexpr unsigned int MAX_LEVELS = 256; // Maximum number of levels in a site
    constexpr unsigned int MAX_ZONES = 1024; // Maximum number of zones in a level
    constexpr unsigned int MAX_POLYGON_POINTS = 4096; // Maximum number of points in a polygon defining an area or zone
    constexpr unsigned int MAX_SENSORS_PER_ASSET = 32; // Maximum number of sensors connected to an asset
    constexpr unsigned int MAX_FEATURES_PER_APPLICATION = 1024; // Maximum number of devices connected to an asset
    constexpr unsigned int MAX_APPLICATION_PERMISSION_PER_ROLE = 1024; // Maximum number of devices connected to an asset

    // Basic data types and structures used in the model
    typedef struct coordinate_t
    {
        double latitude;  // Latitude in degrees
        double longitude; // Longitude in degrees
        double altitude;  // Altitude in meters
    } coordinate_t;

    typedef enum site_type_t
    {
        SITE_TYPE_INDOOR = 'I', // Indoor site
        SITE_TYPE_OUTDOOR = 'O' // Outdoor site
    } site_type_t;

    typedef enum alarm_type_t
    {
        ALARM_TYPE_CRITICAL = 'C', // Critical alarm
        ALARM_TYPE_WARNING = 'W',  // Warning alarm
        ALARM_TYPE_INFO = 'I'      // Informational alarm
    } alarm_type_t;

    typedef enum notification_type_t
    {
        NOTIFICATION_TYPE_ALARM = 'A', // Alarm notification
        NOTIFICATION_TYPE_TELEMETRY = 'T', // Telemetry notification
        NOTIFICATION_TYPE_EVENT = 'E' // Event notification
    } notification_type_t;

    typedef enum telemetry_data_type_t
    {
        TELEMETRY_DATA_TYPE_STRING = 'S', // String data type
        TELEMETRY_DATA_TYPE_NUMBER = 'N', // Numeric data type
        TELEMETRY_DATA_TYPE_BOOLEAN = 'B', // Boolean data type
        TELEMETRY_DATA_TYPE_LOCATION = 'L' // Location data type
    } telemetry_data_type_t;

    typedef enum permission_type_t
    {
        PERMISSION_READ = 0,    // Read permission
        PERMISSION_WRITE = 1,   // Write permission
        PERMISSION_EXECUTE = 2, // Execute permission
        PERMISSION_DELETE = 3,  // Delete permission
        PERMISSION_ADMIN = 4,   // Admin permission
        PERMISSION_CUSTOM = 5   // Custom permission defined by the user
    } permission_type_t;

    // Device related structures
    typedef enum device_type_t
    {
        DEVICE_TYPE_SENSOR = 0,      // Sensor device
        DEVICE_TYPE_ACTUATOR = 1,    // Actuator device
        DEVICE_TYPE_CONTROLLER = 2,   // Controller device
        DEVICE_TYPE_GATEWAY = 3,      // Gateway device
        DEVICE_TYPE_VIRTUAL = 4,      // Virtual device
        DEVICE_TYPE_OTHER = 5         // Other type of device
    } device_type_t;

    typedef enum device_sub_type_t
    {
        DEVICE_SUB_TYPE_TEMPERATURE_SENSOR = 0, // Temperature sensor
        DEVICE_SUB_TYPE_HUMIDITY_SENSOR = 1,    // Humidity sensor
        DEVICE_SUB_TYPE_PRESSURE_SENSOR = 2,     // Pressure sensor
        DEVICE_SUB_TYPE_LIGHT_SENSOR = 3,        // Light sensor
        DEVICE_SUB_TYPE_MOTION_SENSOR = 4,       // Motion sensor
        DEVICE_SUB_TYPE_OTHER = 5                 // Other type of device
    } device_sub_type_t;

    typedef enum device_inventory_life_cycle_t
    {
        DEVICE_LIFE_CYCLE_NEW = 0,          // New device
        DEVICE_LIFE_CYCLE_IN_USE = 1,       // Device in use
        DEVICE_LIFE_CYCLE_DECOMMISSIONED = 2,// Device decommissioned
        DEVICE_LIFE_CYCLE_RETIRED = 3,      // Device retired
        DEVICE_LIFE_CYCLE_OTHER = 4          // Other life cycle state
    } device_inventory_life_cycle_t;

    typedef enum device_attribute_type_t
    {
        DEVICE_ATTRIBUTE_TYPE_STRING = 'S', // String attribute
        DEVICE_ATTRIBUTE_TYPE_NUMBER = 'N', // Numeric attribute
        DEVICE_ATTRIBUTE_TYPE_BOOLEAN = 'B', // Boolean attribute
        DEVICE_ATTRIBUTE_TYPE_LOCATION = 'L' // Location attribute
    } device_attribute_type_t;

    typedef enum rule_type_t
    {
        RULE_TYPE_AUTOMATION = 'A', // Automation rule
        RULE_TYPE_CONDITION = 'C',   // Condition rule
        RULE_TYPE_EVENT = 'E'        // Event rule
    } rule_type_t;

    typedef struct device_t
    {
        char device_id[65];
        char device_name[65];
        char description[257];
        char serial_no[65];
        char hardware_id[65];
        char firmware_version[65];
        char model[65];
        char manufacturer[65];
        device_type_t device_type;
        device_sub_type_t device_sub_type;;
        device_inventory_life_cycle_t device_inventory_life_cycle;
        bool is_active;
        bool is_connected;
        bool is_configured;
        bool is_system;
    } device_t;

    typedef struct asset_t
    {
        char asset_id[65];
        char asset_name[65];
        char description[257];
        char serial_no[65];
        char hardware_id[65];
        char firmware_version[65];
        char model[65];
        char manufacturer[65];
        char category_id[65];
        char subcategory_id[65];
        char site_id[65];
        char level_id[65];
        bool is_active;
        bool is_system;
    } asset_t;

    // This structure defines the sensor connected to an asset
    typedef struct asset_to_sensor_mapping_t
    {
        char asset_sensor_mapping_id[65];
        char asset_id[65]; // ID of the asset this sensor is connected to
        device_t sensor_list[MAX_SENSORS_PER_ASSET]; // Pointer to the sensor device
        unsigned int sensor_count; // Number of sensors connected to the asset
        unsigned long long unix_timestamp_created;
        unsigned long long unix_timestamp_updated;
        bool is_active;
        bool is_system;
    } asset_to_sensor_mapping_t;

    typedef struct device_hierarchy_t
    {
        char device_hierarchy_id[65];
        char device_hierarchy_name[65];
        char description[257];
        char parent_device_id[65]; // ID of the parent device
        char child_device_id[65]; // ID of the child device
        unsigned long long unix_timestamp_created;
        unsigned long long unix_timestamp_updated;
        bool is_active;
        bool is_system;
    } device_hierarchy_t;

    typedef struct device_permission_t
    {
        char device_permission_id[65];
        char device_permission_name[65];
        char description[257];
        char device_id[65]; // ID of the device this permission applies to
        char user_id[65]; // ID of the user this permission applies to
        permission_type_t permission_type; // Type of permission
        unsigned long long unix_timestamp_created;
        unsigned long long unix_timestamp_updated;
        bool is_active;
        bool is_system;
    } device_permission_t;

    typedef struct device_attribute_t
    {
        char device_attribute_id[65];
        char device_attribute_name[65];
        char description[257];
        char device_id[65]; // ID of the device this attribute applies to
        device_attribute_type_t attribute_type; // 'S' for string, 'N' for number, 'B' for boolean, etc.
        char value_string[257]; // For string attributes
        double value_number; // For numeric attributes
        bool value_boolean; // For boolean attributes
        coordinate_t value_location; // For location attributes
        char unit[33]; // Unit of measurement (e.g., "Celsius", "Fahrenheit", "meters")
        double accuracy; // Accuracy of the measurement in meters
        double precision; // Precision of the measurement in meters
        double range_min; // Minimum value for numeric attributes
        double range_max; // Maximum value for numeric attributes
        bool is_active;
        bool is_system;
    } device_attribute_t;

    // Geographic and spatial structures

    typedef struct area_t
    {
        char area_id[65];
        char area_name[65];
        char description[257];
        coordinate_t area_points[MAX_POLYGON_POINTS]; // Array of points defining the area
        unsigned int area_points_count; // Number of points in the area
        bool is_active;
        bool is_system;
    } area_t;

    typedef struct zone_t
    {
        char zone_id[65];
        char zone_name[65];
        char description[257];
        coordinate_t zone_points[MAX_POLYGON_POINTS]; // Array of points defining the zone
        unsigned int zone_points_count; // Number of points in the zone
        bool is_active;
        bool is_system;
    } zone_t;

    typedef struct level_t
    {
        char level_id[65];
        char level_name[65];
        char description[257];
        unsigned int level_number; // 0 for ground, 1 for first floor, etc.
        area_t bounds; // Area defining the level
        zone_t zones[MAX_ZONES]; // Array of zones in the level
        unsigned int zones_count; // Number of zones in the level
        bool is_active;
        bool is_system;
    } level_t;

    typedef struct site_t
    {
        char site_id[65];
        char site_name[65];
        char description[257];
        site_type_t site_type; //'I' for indoor, 'O' for outdoor
        unsigned int site_level_count; // 0 for indoor, 1 or more for outdoor
        level_t levels[MAX_LEVELS]; // Array of levels in the site
        char subcategory_id[65];
        bool is_active;
        bool is_system;
    } site_t;

    // Application, rules and feature related structures

    typedef struct feature_t
    {
        char feature_id[65];
        char feature_name[65];
        char description[257];
        char application_id[65]; // ID of the application this feature belongs to
        unsigned long long unix_timestamp_created;
        unsigned long long unix_timestamp_updated;
        bool is_active;
        bool is_system;
    } feature_t;    
    
    typedef struct application_t
    {
        char application_id[65];
        char application_name[65];
        char description[257];
        char version[65];
        char vendor[65];
        char category_id[65];
        char subcategory_id[65];
        feature_t features[MAX_FEATURES_PER_APPLICATION]; // Array of features in the application
        unsigned int features_count; // Number of features in the application
        bool is_active;
        bool is_system;
    } application_t;

    // To be used by the rules engine for defining rules
    typedef struct rule_t
    {
        char rule_id[65];
        char rule_name[65];
        char description[257];
        rule_type_t rule_type; // 'A' for automation, 'C' for condition, 'E' for event
        char rule_expression[1025]; // Expression defining the rule logic
        unsigned int priority; // Priority of the rule
        bool is_active;
        bool is_system;
    } rule_t;

    // User, enterprise, permission and role related structures
    typedef struct user_t
    {
        char user_id[65];
        char enterprise_id[65];
        char user_name[65];
        char email[256];
        char contact_mo[32];
        char first_name[65];
        char last_name[65];
        char password_hash[257];
        char password_salt[257];
        unsigned long long unix_timestamp_last_login;
        bool is_active;
        bool is_system;
    } user_t;

    typedef struct enterprise_t
    {
        char enterprise_id[65];
        char enterprise_name[65];
        char description[256];
        char contact_mo[32];
        char contact_email[257];
        char contact_first_name[65];
        char contact_last_name[65];
        unsigned long long unix_timestamp_created;
        char whitelabel_text[1025];
        char address_line1[257];
        char address_line2[257];
        char address_city[33];
        char address_state[33];
        char address_country[33];
        char address_pin_code[17];
        bool is_active;
        bool is_system;
    } enterprise_t;

    // Describes a application permission mapping
    typedef struct application_permission_t
    {
        char application_permission_id[65];
        char application_id[65]; // ID of the application this permission applies to
        char user_id[65]; // ID of the user this permission applies to
        permission_type_t permission_type; // Type of permission
        unsigned long long unix_timestamp_created;
        unsigned long long unix_timestamp_updated;
        bool is_active;
        bool is_system;
    } application_permission_t;

    typedef struct role_t
    {
        char role_id[65];
        char role_name[65];
        char description[257];
        application_permission_t permissions[MAX_APPLICATION_PERMISSION_PER_ROLE]; // Array of application permissions associated with the role
        unsigned int permissions_count; // Number of application permissions in the role
        unsigned long long unix_timestamp_created;
        unsigned long long unix_timestamp_updated;
        bool is_active;
        bool is_system;
    } role_t;

    typedef struct user_role_mapping_t
    {
        char user_role_mapping_id[65];
        char user_id[65]; // ID of the user
        char role_id[65]; // ID of the role
        unsigned long long unix_timestamp_created;
        unsigned long long unix_timestamp_updated;
        bool is_active;
        bool is_system;
    } user_role_mapping_t;

    typedef struct login_session_t
    {
        char session_id[65];
        char user_id[65]; // ID of the user this session belongs to
        unsigned long long unix_timestamp_created;
        unsigned long long unix_timestamp_expires;
        coordinate_t location; // Location of the user during login
        bool is_active;
        bool is_system;
    } login_session_t;

    // Telemetry and data structures
    typedef struct telemetry_data_t
    {
        char telemetry_data_id[65];
        char device_id[65]; // ID of the device this telemetry data belongs to
        unsigned long long unix_timestamp; // Timestamp of the telemetry data
        telemetry_data_type_t data_type; // 'S' for string, 'N' for number, 'B' for boolean, etc.
        char value_string[257]; // For string data
        double value_number; // For numeric data
        bool value_boolean; // For boolean data
        coordinate_t value_location; // For location data
        char unit[33]; // Unit of measurement (e.g., "Celsius", "Fahrenheit", "meters")
        double accuracy; // Accuracy of the measurement in meters
        double precision; // Precision of the measurement in meters
        double range_min; // Minimum value for numeric data
        double range_max; // Maximum value for numeric data
        bool is_active;
        bool is_system;
    } telemetry_data_t;

    typedef struct alarm_t
    {
        char alarm_id[65];
        char device_id[65]; // ID of the device this alarm belongs to
        unsigned long long unix_timestamp; // Timestamp of the alarm
        alarm_type_t alarm_type; // 'C' for critical, 'W' for warning, 'I' for info
        char description[257]; // Description of the alarm
        telemetry_data_t* related_telemetry; // Related telemetry data
        unsigned int related_telemetry_count; // Number of related telemetry data
        bool is_active;
        bool is_system;
    } alarm_t;  

    typedef struct notification_t
    {
        char notification_id[65];
        char user_id[65]; // ID of the user this notification belongs to
        unsigned long long unix_timestamp; // Timestamp of the notification
        notification_type_t notification_type; // 'A' for alarm, 'T' for telemetry, 'E' for event
        char description[257]; // Description of the notification
        bool is_read; // Whether the notification has been read
        bool is_active;
        bool is_system;
    } notification_t;   

}

#endif
