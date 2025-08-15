-- IoT Data Model - PostgreSQL Schema
-- Generated from C structures for comprehensive IoT platform

-- Drop existing tables (in reverse dependency order)
DROP TABLE IF EXISTS notifications CASCADE;
DROP TABLE IF EXISTS alarms CASCADE;
DROP TABLE IF EXISTS alarm_telemetry_mapping CASCADE;
DROP TABLE IF EXISTS telemetry_data CASCADE;
DROP TABLE IF EXISTS login_sessions CASCADE;
DROP TABLE IF EXISTS user_role_mappings CASCADE;
DROP TABLE IF EXISTS role_application_permissions CASCADE;
DROP TABLE IF EXISTS roles CASCADE;
DROP TABLE IF EXISTS application_permissions CASCADE;
DROP TABLE IF EXISTS device_permissions CASCADE;
DROP TABLE IF EXISTS rules CASCADE;
DROP TABLE IF EXISTS application_features CASCADE;
DROP TABLE IF EXISTS applications CASCADE;
DROP TABLE IF EXISTS device_attributes CASCADE;
DROP TABLE IF EXISTS device_hierarchy CASCADE;
DROP TABLE IF EXISTS asset_sensor_mappings CASCADE;
DROP TABLE IF EXISTS asset_sensors CASCADE;
DROP TABLE IF EXISTS assets CASCADE;
DROP TABLE IF EXISTS devices CASCADE;
DROP TABLE IF EXISTS zones CASCADE;
DROP TABLE IF EXISTS levels CASCADE;
DROP TABLE IF EXISTS sites CASCADE;
DROP TABLE IF EXISTS users CASCADE;
DROP TABLE IF EXISTS enterprises CASCADE;
DROP TABLE IF EXISTS categories CASCADE;
DROP TABLE IF EXISTS subcategories CASCADE;

-- Create ENUM types
CREATE TYPE site_type_enum AS ENUM ('I', 'O'); -- Indoor, Outdoor
CREATE TYPE alarm_type_enum AS ENUM ('C', 'W', 'I'); -- Critical, Warning, Info
CREATE TYPE notification_type_enum AS ENUM ('A', 'T', 'E'); -- Alarm, Telemetry, Event
CREATE TYPE telemetry_data_type_enum AS ENUM ('S', 'N', 'B', 'L'); -- String, Number, Boolean, Location
CREATE TYPE device_attribute_type_enum AS ENUM ('S', 'N', 'B', 'L'); -- String, Number, Boolean, Location
CREATE TYPE permission_type_enum AS ENUM ('READ', 'WRITE', 'EXECUTE', 'DELETE', 'ADMIN', 'CUSTOM');
CREATE TYPE device_type_enum AS ENUM ('SENSOR', 'ACTUATOR', 'CONTROLLER', 'GATEWAY', 'VIRTUAL', 'OTHER');
CREATE TYPE device_sub_type_enum AS ENUM ('TEMPERATURE_SENSOR', 'HUMIDITY_SENSOR', 'PRESSURE_SENSOR', 'LIGHT_SENSOR', 'MOTION_SENSOR', 'OTHER');
CREATE TYPE device_lifecycle_enum AS ENUM ('NEW', 'IN_USE', 'DECOMMISSIONED', 'RETIRED', 'OTHER');
CREATE TYPE rule_type_enum AS ENUM ('A', 'C', 'E'); -- Automation, Condition, Event

-- Categories and Subcategories (referenced by various entities)
CREATE TABLE categories (
    category_id VARCHAR(64) PRIMARY KEY,
    category_name VARCHAR(64) NOT NULL,
    description VARCHAR(256),
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false
);

CREATE TABLE subcategories (
    subcategory_id VARCHAR(64) PRIMARY KEY,
    subcategory_name VARCHAR(64) NOT NULL,
    category_id VARCHAR(64) NOT NULL REFERENCES categories(category_id),
    description VARCHAR(256),
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false
);

-- Enterprise and User Management
CREATE TABLE enterprises (
    enterprise_id VARCHAR(64) PRIMARY KEY,
    enterprise_name VARCHAR(64) NOT NULL,
    description VARCHAR(255),
    contact_mo VARCHAR(31),
    contact_email VARCHAR(256),
    contact_first_name VARCHAR(64),
    contact_last_name VARCHAR(64),
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    whitelabel_text VARCHAR(1024),
    address_line1 VARCHAR(256),
    address_line2 VARCHAR(256),
    address_city VARCHAR(32),
    address_state VARCHAR(32),
    address_country VARCHAR(32),
    address_pin_code VARCHAR(16),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false
);

CREATE TABLE users (
    user_id VARCHAR(64) PRIMARY KEY,
    enterprise_id VARCHAR(64) NOT NULL REFERENCES enterprises(enterprise_id),
    user_name VARCHAR(64) NOT NULL,
    email VARCHAR(255) NOT NULL,
    contact_mo VARCHAR(31),
    first_name VARCHAR(64),
    last_name VARCHAR(64),
    password_hash VARCHAR(256),
    password_salt VARCHAR(256),
    unix_timestamp_last_login BIGINT,
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false,
    UNIQUE(enterprise_id, user_name),
    UNIQUE(enterprise_id, email)
);

-- Geographic and Spatial Structures
CREATE TABLE sites (
    site_id VARCHAR(64) PRIMARY KEY,
    site_name VARCHAR(64) NOT NULL,
    description VARCHAR(256),
    site_type site_type_enum NOT NULL,
    site_level_count INTEGER NOT NULL DEFAULT 0,
    subcategory_id VARCHAR(64) REFERENCES subcategories(subcategory_id),
    enterprise_id VARCHAR(64) NOT NULL REFERENCES enterprises(enterprise_id),
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false
);

CREATE TABLE levels (
    level_id VARCHAR(64) PRIMARY KEY,
    level_name VARCHAR(64) NOT NULL,
    site_id VARCHAR(64) NOT NULL REFERENCES sites(site_id) ON DELETE CASCADE,
    description VARCHAR(256),
    level_number INTEGER NOT NULL,
    -- Bounds area defined as polygon points
    bounds_points_count INTEGER NOT NULL DEFAULT 0,
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false,
    UNIQUE(site_id, level_number)
);

-- Level bounds points (replaces embedded bounds area)
CREATE TABLE level_bounds_points (
    level_id VARCHAR(64) NOT NULL REFERENCES levels(level_id) ON DELETE CASCADE,
    point_index INTEGER NOT NULL,
    latitude DOUBLE PRECISION NOT NULL,
    longitude DOUBLE PRECISION NOT NULL,
    altitude DOUBLE PRECISION NOT NULL DEFAULT 0,
    PRIMARY KEY (level_id, point_index)
);

CREATE TABLE zones (
    zone_id VARCHAR(64) PRIMARY KEY,
    zone_name VARCHAR(64) NOT NULL,
    level_id VARCHAR(64) NOT NULL REFERENCES levels(level_id) ON DELETE CASCADE,
    description VARCHAR(256),
    zone_points_count INTEGER NOT NULL DEFAULT 0,
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false
);

-- Zone boundary points
CREATE TABLE zone_points (
    zone_id VARCHAR(64) NOT NULL REFERENCES zones(zone_id) ON DELETE CASCADE,
    point_index INTEGER NOT NULL,
    latitude DOUBLE PRECISION NOT NULL,
    longitude DOUBLE PRECISION NOT NULL,
    altitude DOUBLE PRECISION NOT NULL DEFAULT 0,
    PRIMARY KEY (zone_id, point_index)
);

-- Device and Asset Management
CREATE TABLE devices (
    device_id VARCHAR(64) PRIMARY KEY,
    device_name VARCHAR(64) NOT NULL,
    description VARCHAR(256),
    serial_no VARCHAR(64),
    hardware_id VARCHAR(64),
    firmware_version VARCHAR(64),
    model VARCHAR(64),
    manufacturer VARCHAR(64),
    device_type device_type_enum NOT NULL,
    device_sub_type device_sub_type_enum NOT NULL,
    device_inventory_life_cycle device_lifecycle_enum NOT NULL DEFAULT 'NEW',
    enterprise_id VARCHAR(64) NOT NULL REFERENCES enterprises(enterprise_id),
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_connected BOOLEAN NOT NULL DEFAULT false,
    is_configured BOOLEAN NOT NULL DEFAULT false,
    is_system BOOLEAN NOT NULL DEFAULT false
);

CREATE TABLE assets (
    asset_id VARCHAR(64) PRIMARY KEY,
    asset_name VARCHAR(64) NOT NULL,
    description VARCHAR(256),
    serial_no VARCHAR(64),
    hardware_id VARCHAR(64),
    firmware_version VARCHAR(64),
    model VARCHAR(64),
    manufacturer VARCHAR(64),
    category_id VARCHAR(64) REFERENCES categories(category_id),
    subcategory_id VARCHAR(64) REFERENCES subcategories(subcategory_id),
    site_id VARCHAR(64) NOT NULL REFERENCES sites(site_id),
    level_id VARCHAR(64) REFERENCES levels(level_id),
    enterprise_id VARCHAR(64) NOT NULL REFERENCES enterprises(enterprise_id),
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false
);

-- Asset to Sensor mappings (replaces embedded sensor arrays)
CREATE TABLE asset_sensor_mappings (
    asset_sensor_mapping_id VARCHAR(64) PRIMARY KEY,
    asset_id VARCHAR(64) NOT NULL REFERENCES assets(asset_id) ON DELETE CASCADE,
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false
);

CREATE TABLE asset_sensors (
    asset_sensor_mapping_id VARCHAR(64) NOT NULL REFERENCES asset_sensor_mappings(asset_sensor_mapping_id) ON DELETE CASCADE,
    device_id VARCHAR(64) NOT NULL REFERENCES devices(device_id),
    sensor_index INTEGER NOT NULL,
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    PRIMARY KEY (asset_sensor_mapping_id, sensor_index),
    UNIQUE(asset_sensor_mapping_id, device_id)
);

CREATE TABLE device_hierarchy (
    device_hierarchy_id VARCHAR(64) PRIMARY KEY,
    device_hierarchy_name VARCHAR(64) NOT NULL,
    description VARCHAR(256),
    parent_device_id VARCHAR(64) NOT NULL REFERENCES devices(device_id),
    child_device_id VARCHAR(64) NOT NULL REFERENCES devices(device_id),
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false,
    UNIQUE(parent_device_id, child_device_id),
    CHECK (parent_device_id != child_device_id)
);

CREATE TABLE device_attributes (
    device_attribute_id VARCHAR(64) PRIMARY KEY,
    device_attribute_name VARCHAR(64) NOT NULL,
    description VARCHAR(256),
    device_id VARCHAR(64) NOT NULL REFERENCES devices(device_id) ON DELETE CASCADE,
    attribute_type device_attribute_type_enum NOT NULL,
    value_string VARCHAR(256),
    value_number DOUBLE PRECISION,
    value_boolean BOOLEAN,
    value_location_latitude DOUBLE PRECISION,
    value_location_longitude DOUBLE PRECISION,
    value_location_altitude DOUBLE PRECISION,
    unit VARCHAR(32),
    accuracy DOUBLE PRECISION,
    precision_value DOUBLE PRECISION, -- 'precision' is reserved keyword
    range_min DOUBLE PRECISION,
    range_max DOUBLE PRECISION,
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false
);

-- Application and Features
CREATE TABLE applications (
    application_id VARCHAR(64) PRIMARY KEY,
    application_name VARCHAR(64) NOT NULL,
    description VARCHAR(256),
    version VARCHAR(64),
    vendor VARCHAR(64),
    category_id VARCHAR(64) REFERENCES categories(category_id),
    subcategory_id VARCHAR(64) REFERENCES subcategories(subcategory_id),
    enterprise_id VARCHAR(64) NOT NULL REFERENCES enterprises(enterprise_id),
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false
);

CREATE TABLE application_features (
    feature_id VARCHAR(64) PRIMARY KEY,
    feature_name VARCHAR(64) NOT NULL,
    description VARCHAR(256),
    application_id VARCHAR(64) NOT NULL REFERENCES applications(application_id) ON DELETE CASCADE,
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false
);

-- Rules Engine
CREATE TABLE rules (
    rule_id VARCHAR(64) PRIMARY KEY,
    rule_name VARCHAR(64) NOT NULL,
    description VARCHAR(256),
    rule_type rule_type_enum NOT NULL,
    rule_expression VARCHAR(1024) NOT NULL,
    priority INTEGER NOT NULL DEFAULT 0,
    enterprise_id VARCHAR(64) NOT NULL REFERENCES enterprises(enterprise_id),
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false
);

-- Permission and Role Management
CREATE TABLE device_permissions (
    device_permission_id VARCHAR(64) PRIMARY KEY,
    device_permission_name VARCHAR(64) NOT NULL,
    description VARCHAR(256),
    device_id VARCHAR(64) NOT NULL REFERENCES devices(device_id) ON DELETE CASCADE,
    user_id VARCHAR(64) NOT NULL REFERENCES users(user_id) ON DELETE CASCADE,
    permission_type permission_type_enum NOT NULL,
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false,
    UNIQUE(device_id, user_id, permission_type)
);

CREATE TABLE application_permissions (
    application_permission_id VARCHAR(64) PRIMARY KEY,
    application_id VARCHAR(64) NOT NULL REFERENCES applications(application_id) ON DELETE CASCADE,
    user_id VARCHAR(64) NOT NULL REFERENCES users(user_id) ON DELETE CASCADE,
    permission_type permission_type_enum NOT NULL,
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false,
    UNIQUE(application_id, user_id, permission_type)
);

CREATE TABLE roles (
    role_id VARCHAR(64) PRIMARY KEY,
    role_name VARCHAR(64) NOT NULL,
    description VARCHAR(256),
    enterprise_id VARCHAR(64) NOT NULL REFERENCES enterprises(enterprise_id),
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false,
    UNIQUE(enterprise_id, role_name)
);

CREATE TABLE role_application_permissions (
    role_id VARCHAR(64) NOT NULL REFERENCES roles(role_id) ON DELETE CASCADE,
    application_permission_id VARCHAR(64) NOT NULL REFERENCES application_permissions(application_permission_id) ON DELETE CASCADE,
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    PRIMARY KEY (role_id, application_permission_id)
);

CREATE TABLE user_role_mappings (
    user_role_mapping_id VARCHAR(64) PRIMARY KEY,
    user_id VARCHAR(64) NOT NULL REFERENCES users(user_id) ON DELETE CASCADE,
    role_id VARCHAR(64) NOT NULL REFERENCES roles(role_id) ON DELETE CASCADE,
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_updated BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false,
    UNIQUE(user_id, role_id)
);

CREATE TABLE login_sessions (
    session_id VARCHAR(64) PRIMARY KEY,
    user_id VARCHAR(64) NOT NULL REFERENCES users(user_id) ON DELETE CASCADE,
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    unix_timestamp_expires BIGINT NOT NULL,
    location_latitude DOUBLE PRECISION,
    location_longitude DOUBLE PRECISION,
    location_altitude DOUBLE PRECISION DEFAULT 0,
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false
);

-- Telemetry and Data
CREATE TABLE telemetry_data (
    telemetry_data_id VARCHAR(64) PRIMARY KEY,
    device_id VARCHAR(64) NOT NULL REFERENCES devices(device_id),
    unix_timestamp BIGINT NOT NULL,
    data_type telemetry_data_type_enum NOT NULL,
    value_string VARCHAR(256),
    value_number DOUBLE PRECISION,
    value_boolean BOOLEAN,
    value_location_latitude DOUBLE PRECISION,
    value_location_longitude DOUBLE PRECISION,
    value_location_altitude DOUBLE PRECISION,
    unit VARCHAR(32),
    accuracy DOUBLE PRECISION,
    precision_value DOUBLE PRECISION,
    range_min DOUBLE PRECISION,
    range_max DOUBLE PRECISION,
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false
);

CREATE TABLE alarms (
    alarm_id VARCHAR(64) PRIMARY KEY,
    device_id VARCHAR(64) NOT NULL REFERENCES devices(device_id),
    unix_timestamp BIGINT NOT NULL,
    alarm_type alarm_type_enum NOT NULL,
    description VARCHAR(256),
    related_telemetry_count INTEGER NOT NULL DEFAULT 0,
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false
);

-- Alarm to Telemetry mapping (replaces pointer in C struct)
CREATE TABLE alarm_telemetry_mapping (
    alarm_id VARCHAR(64) NOT NULL REFERENCES alarms(alarm_id) ON DELETE CASCADE,
    telemetry_data_id VARCHAR(64) NOT NULL REFERENCES telemetry_data(telemetry_data_id),
    mapping_index INTEGER NOT NULL,
    unix_timestamp_created BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM NOW()),
    PRIMARY KEY (alarm_id, telemetry_data_id),
    UNIQUE(alarm_id, mapping_index)
);

CREATE TABLE notifications (
    notification_id VARCHAR(64) PRIMARY KEY,
    user_id VARCHAR(64) NOT NULL REFERENCES users(user_id) ON DELETE CASCADE,
    unix_timestamp BIGINT NOT NULL,
    notification_type notification_type_enum NOT NULL,
    description VARCHAR(256),
    is_read BOOLEAN NOT NULL DEFAULT false,
    is_active BOOLEAN NOT NULL DEFAULT true,
    is_system BOOLEAN NOT NULL DEFAULT false
);

-- Create indexes for performance
CREATE INDEX idx_users_enterprise ON users(enterprise_id);
CREATE INDEX idx_users_email ON users(email);
CREATE INDEX idx_sites_enterprise ON sites(enterprise_id);
CREATE INDEX idx_levels_site ON levels(site_id);
CREATE INDEX idx_zones_level ON zones(level_id);
CREATE INDEX idx_devices_enterprise ON devices(enterprise_id);
CREATE INDEX idx_devices_type ON devices(device_type);
CREATE INDEX idx_assets_site ON assets(site_id);
CREATE INDEX idx_assets_enterprise ON assets(enterprise_id);
CREATE INDEX idx_telemetry_device ON telemetry_data(device_id);
CREATE INDEX idx_telemetry_timestamp ON telemetry_data(unix_timestamp);
CREATE INDEX idx_telemetry_device_timestamp ON telemetry_data(device_id, unix_timestamp);
CREATE INDEX idx_alarms_device ON alarms(device_id);
CREATE INDEX idx_alarms_timestamp ON alarms(unix_timestamp);
CREATE INDEX idx_notifications_user ON notifications(user_id);
CREATE INDEX idx_notifications_unread ON notifications(user_id, is_read) WHERE is_read = false;
CREATE INDEX idx_login_sessions_user ON login_sessions(user_id);
CREATE INDEX idx_device_attributes_device ON device_attributes(device_id);
CREATE INDEX idx_applications_enterprise ON applications(enterprise_id);
CREATE INDEX idx_roles_enterprise ON roles(enterprise_id);

-- Create triggers for updating timestamp fields
CREATE OR REPLACE FUNCTION update_timestamp()
RETURNS TRIGGER AS $$
BEGIN
    NEW.unix_timestamp_updated = EXTRACT(EPOCH FROM NOW());
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- Apply update triggers to relevant tables
CREATE TRIGGER tr_users_updated BEFORE UPDATE ON users
    FOR EACH ROW EXECUTE FUNCTION update_timestamp();

CREATE TRIGGER tr_enterprises_updated BEFORE UPDATE ON enterprises
    FOR EACH ROW EXECUTE FUNCTION update_timestamp();

CREATE TRIGGER tr_sites_updated BEFORE UPDATE ON sites
    FOR EACH ROW EXECUTE FUNCTION update_timestamp();

CREATE TRIGGER tr_levels_updated BEFORE UPDATE ON levels
    FOR EACH ROW EXECUTE FUNCTION update_timestamp();

CREATE TRIGGER tr_zones_updated BEFORE UPDATE ON zones
    FOR EACH ROW EXECUTE FUNCTION update_timestamp();

CREATE TRIGGER tr_devices_updated BEFORE UPDATE ON devices
    FOR EACH ROW EXECUTE FUNCTION update_timestamp();

CREATE TRIGGER tr_assets_updated BEFORE UPDATE ON assets
    FOR EACH ROW EXECUTE FUNCTION update_timestamp();

-- Add comments for documentation
COMMENT ON DATABASE edgelite IS 'IoT Platform Data Model - Comprehensive schema for device management, telemetry, and user permissions';
COMMENT ON TABLE enterprises IS 'Multi-tenant enterprise/organization management';
COMMENT ON TABLE users IS 'User accounts with enterprise isolation';
COMMENT ON TABLE sites IS 'Physical or logical sites containing IoT deployments';
COMMENT ON TABLE devices IS 'IoT devices including sensors, actuators, gateways';
COMMENT ON TABLE telemetry_data IS 'Time-series data from IoT devices with flexible data types';
COMMENT ON TABLE alarms IS 'Alert system for device anomalies and events';
COMMENT ON TABLE roles IS 'Role-based access control for applications and features';
