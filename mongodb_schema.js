// IoT Data Model - MongoDB Schema
// Generated from C structures for comprehensive IoT platform
// This script creates collections, indexes, and validation rules

// Use the IoT database
use('edgelite');

// Drop existing collections (careful in production!)
db.notifications.drop();
db.alarms.drop();
db.telemetryData.drop();
db.loginSessions.drop();
db.userRoleMappings.drop();
db.roles.drop();
db.applicationPermissions.drop();
db.devicePermissions.drop();
db.rules.drop();
db.applicationFeatures.drop();
db.applications.drop();
db.deviceAttributes.drop();
db.deviceHierarchy.drop();
db.assetSensorMappings.drop();
db.assets.drop();
db.devices.drop();
db.zones.drop();
db.levels.drop();
db.sites.drop();
db.users.drop();
db.enterprises.drop();
db.categories.drop();
db.subcategories.drop();

// Create collections with validation schemas
db.createCollection("categories", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["categoryId", "categoryName", "unixTimestampCreated"],
      properties: {
        categoryId: { bsonType: "string", maxLength: 64 },
        categoryName: { bsonType: "string", maxLength: 64 },
        description: { bsonType: "string", maxLength: 256 },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("subcategories", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["subcategoryId", "subcategoryName", "categoryId", "unixTimestampCreated"],
      properties: {
        subcategoryId: { bsonType: "string", maxLength: 64 },
        subcategoryName: { bsonType: "string", maxLength: 64 },
        categoryId: { bsonType: "string", maxLength: 64 },
        description: { bsonType: "string", maxLength: 256 },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("enterprises", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["enterpriseId", "enterpriseName", "unixTimestampCreated"],
      properties: {
        enterpriseId: { bsonType: "string", maxLength: 64 },
        enterpriseName: { bsonType: "string", maxLength: 64 },
        description: { bsonType: "string", maxLength: 255 },
        contactMo: { bsonType: "string", maxLength: 31 },
        contactEmail: { bsonType: "string", maxLength: 256 },
        contactFirstName: { bsonType: "string", maxLength: 64 },
        contactLastName: { bsonType: "string", maxLength: 64 },
        unixTimestampCreated: { bsonType: "long" },
        whitelabelText: { bsonType: "string", maxLength: 1024 },
        address: {
          bsonType: "object",
          properties: {
            line1: { bsonType: "string", maxLength: 256 },
            line2: { bsonType: "string", maxLength: 256 },
            city: { bsonType: "string", maxLength: 32 },
            state: { bsonType: "string", maxLength: 32 },
            country: { bsonType: "string", maxLength: 32 },
            pinCode: { bsonType: "string", maxLength: 16 }
          }
        },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("users", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["userId", "enterpriseId", "userName", "email", "unixTimestampCreated"],
      properties: {
        userId: { bsonType: "string", maxLength: 64 },
        enterpriseId: { bsonType: "string", maxLength: 64 },
        userName: { bsonType: "string", maxLength: 64 },
        email: { bsonType: "string", maxLength: 255 },
        contactMo: { bsonType: "string", maxLength: 31 },
        firstName: { bsonType: "string", maxLength: 64 },
        lastName: { bsonType: "string", maxLength: 64 },
        passwordHash: { bsonType: "string", maxLength: 256 },
        passwordSalt: { bsonType: "string", maxLength: 256 },
        unixTimestampLastLogin: { bsonType: "long" },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("sites", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["siteId", "siteName", "siteType", "enterpriseId", "unixTimestampCreated"],
      properties: {
        siteId: { bsonType: "string", maxLength: 64 },
        siteName: { bsonType: "string", maxLength: 64 },
        description: { bsonType: "string", maxLength: 256 },
        siteType: { enum: ["I", "O"] }, // Indoor/Outdoor
        siteLevelCount: { bsonType: "int", minimum: 0 },
        subcategoryId: { bsonType: "string", maxLength: 64 },
        enterpriseId: { bsonType: "string", maxLength: 64 },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("levels", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["levelId", "levelName", "siteId", "levelNumber", "unixTimestampCreated"],
      properties: {
        levelId: { bsonType: "string", maxLength: 64 },
        levelName: { bsonType: "string", maxLength: 64 },
        siteId: { bsonType: "string", maxLength: 64 },
        description: { bsonType: "string", maxLength: 256 },
        levelNumber: { bsonType: "int" },
        bounds: {
          bsonType: "object",
          properties: {
            areaId: { bsonType: "string", maxLength: 64 },
            areaName: { bsonType: "string", maxLength: 64 },
            description: { bsonType: "string", maxLength: 256 },
            areaPoints: {
              bsonType: "array",
              maxItems: 4096,
              items: {
                bsonType: "object",
                required: ["latitude", "longitude"],
                properties: {
                  latitude: { bsonType: "double" },
                  longitude: { bsonType: "double" },
                  altitude: { bsonType: "double" }
                }
              }
            },
            areaPointsCount: { bsonType: "int", minimum: 0, maximum: 4096 },
            isActive: { bsonType: "bool" },
            isSystem: { bsonType: "bool" }
          }
        },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("zones", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["zoneId", "zoneName", "levelId", "unixTimestampCreated"],
      properties: {
        zoneId: { bsonType: "string", maxLength: 64 },
        zoneName: { bsonType: "string", maxLength: 64 },
        levelId: { bsonType: "string", maxLength: 64 },
        description: { bsonType: "string", maxLength: 256 },
        zonePoints: {
          bsonType: "array",
          maxItems: 4096,
          items: {
            bsonType: "object",
            required: ["latitude", "longitude"],
            properties: {
              latitude: { bsonType: "double" },
              longitude: { bsonType: "double" },
              altitude: { bsonType: "double" }
            }
          }
        },
        zonePointsCount: { bsonType: "int", minimum: 0, maximum: 4096 },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("devices", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["deviceId", "deviceName", "deviceType", "deviceSubType", "enterpriseId", "unixTimestampCreated"],
      properties: {
        deviceId: { bsonType: "string", maxLength: 64 },
        deviceName: { bsonType: "string", maxLength: 64 },
        description: { bsonType: "string", maxLength: 256 },
        serialNo: { bsonType: "string", maxLength: 64 },
        hardwareId: { bsonType: "string", maxLength: 64 },
        firmwareVersion: { bsonType: "string", maxLength: 64 },
        model: { bsonType: "string", maxLength: 64 },
        manufacturer: { bsonType: "string", maxLength: 64 },
        deviceType: { enum: ["SENSOR", "ACTUATOR", "CONTROLLER", "GATEWAY", "VIRTUAL", "OTHER"] },
        deviceSubType: { enum: ["TEMPERATURE_SENSOR", "HUMIDITY_SENSOR", "PRESSURE_SENSOR", "LIGHT_SENSOR", "MOTION_SENSOR", "OTHER"] },
        deviceInventoryLifeCycle: { enum: ["NEW", "IN_USE", "DECOMMISSIONED", "RETIRED", "OTHER"] },
        enterpriseId: { bsonType: "string", maxLength: 64 },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isConnected: { bsonType: "bool" },
        isConfigured: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("assets", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["assetId", "assetName", "siteId", "enterpriseId", "unixTimestampCreated"],
      properties: {
        assetId: { bsonType: "string", maxLength: 64 },
        assetName: { bsonType: "string", maxLength: 64 },
        description: { bsonType: "string", maxLength: 256 },
        serialNo: { bsonType: "string", maxLength: 64 },
        hardwareId: { bsonType: "string", maxLength: 64 },
        firmwareVersion: { bsonType: "string", maxLength: 64 },
        model: { bsonType: "string", maxLength: 64 },
        manufacturer: { bsonType: "string", maxLength: 64 },
        categoryId: { bsonType: "string", maxLength: 64 },
        subcategoryId: { bsonType: "string", maxLength: 64 },
        siteId: { bsonType: "string", maxLength: 64 },
        levelId: { bsonType: "string", maxLength: 64 },
        enterpriseId: { bsonType: "string", maxLength: 64 },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("assetSensorMappings", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["assetSensorMappingId", "assetId", "unixTimestampCreated"],
      properties: {
        assetSensorMappingId: { bsonType: "string", maxLength: 64 },
        assetId: { bsonType: "string", maxLength: 64 },
        sensorList: {
          bsonType: "array",
          maxItems: 32,
          items: {
            bsonType: "object",
            required: ["deviceId"],
            properties: {
              deviceId: { bsonType: "string", maxLength: 64 },
              sensorIndex: { bsonType: "int", minimum: 0 },
              unixTimestampAdded: { bsonType: "long" },
              isActive: { bsonType: "bool" }
            }
          }
        },
        sensorCount: { bsonType: "int", minimum: 0, maximum: 32 },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("deviceHierarchy", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["deviceHierarchyId", "deviceHierarchyName", "parentDeviceId", "childDeviceId", "unixTimestampCreated"],
      properties: {
        deviceHierarchyId: { bsonType: "string", maxLength: 64 },
        deviceHierarchyName: { bsonType: "string", maxLength: 64 },
        description: { bsonType: "string", maxLength: 256 },
        parentDeviceId: { bsonType: "string", maxLength: 64 },
        childDeviceId: { bsonType: "string", maxLength: 64 },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("deviceAttributes", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["deviceAttributeId", "deviceAttributeName", "deviceId", "attributeType", "unixTimestampCreated"],
      properties: {
        deviceAttributeId: { bsonType: "string", maxLength: 64 },
        deviceAttributeName: { bsonType: "string", maxLength: 64 },
        description: { bsonType: "string", maxLength: 256 },
        deviceId: { bsonType: "string", maxLength: 64 },
        attributeType: { enum: ["S", "N", "B", "L"] }, // String, Number, Boolean, Location
        valueString: { bsonType: "string", maxLength: 256 },
        valueNumber: { bsonType: "double" },
        valueBoolean: { bsonType: "bool" },
        valueLocation: {
          bsonType: "object",
          properties: {
            latitude: { bsonType: "double" },
            longitude: { bsonType: "double" },
            altitude: { bsonType: "double" }
          }
        },
        unit: { bsonType: "string", maxLength: 32 },
        accuracy: { bsonType: "double" },
        precision: { bsonType: "double" },
        rangeMin: { bsonType: "double" },
        rangeMax: { bsonType: "double" },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("applications", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["applicationId", "applicationName", "enterpriseId", "unixTimestampCreated"],
      properties: {
        applicationId: { bsonType: "string", maxLength: 64 },
        applicationName: { bsonType: "string", maxLength: 64 },
        description: { bsonType: "string", maxLength: 256 },
        version: { bsonType: "string", maxLength: 64 },
        vendor: { bsonType: "string", maxLength: 64 },
        categoryId: { bsonType: "string", maxLength: 64 },
        subcategoryId: { bsonType: "string", maxLength: 64 },
        enterpriseId: { bsonType: "string", maxLength: 64 },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("applicationFeatures", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["featureId", "featureName", "applicationId", "unixTimestampCreated"],
      properties: {
        featureId: { bsonType: "string", maxLength: 64 },
        featureName: { bsonType: "string", maxLength: 64 },
        description: { bsonType: "string", maxLength: 256 },
        applicationId: { bsonType: "string", maxLength: 64 },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("rules", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["ruleId", "ruleName", "ruleType", "ruleExpression", "enterpriseId", "unixTimestampCreated"],
      properties: {
        ruleId: { bsonType: "string", maxLength: 64 },
        ruleName: { bsonType: "string", maxLength: 64 },
        description: { bsonType: "string", maxLength: 256 },
        ruleType: { enum: ["A", "C", "E"] }, // Automation, Condition, Event
        ruleExpression: { bsonType: "string", maxLength: 1024 },
        priority: { bsonType: "int" },
        enterpriseId: { bsonType: "string", maxLength: 64 },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("devicePermissions", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["devicePermissionId", "devicePermissionName", "deviceId", "userId", "permissionType", "unixTimestampCreated"],
      properties: {
        devicePermissionId: { bsonType: "string", maxLength: 64 },
        devicePermissionName: { bsonType: "string", maxLength: 64 },
        description: { bsonType: "string", maxLength: 256 },
        deviceId: { bsonType: "string", maxLength: 64 },
        userId: { bsonType: "string", maxLength: 64 },
        permissionType: { enum: ["READ", "WRITE", "EXECUTE", "DELETE", "ADMIN", "CUSTOM"] },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("applicationPermissions", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["applicationPermissionId", "applicationId", "userId", "permissionType", "unixTimestampCreated"],
      properties: {
        applicationPermissionId: { bsonType: "string", maxLength: 64 },
        applicationId: { bsonType: "string", maxLength: 64 },
        userId: { bsonType: "string", maxLength: 64 },
        permissionType: { enum: ["READ", "WRITE", "EXECUTE", "DELETE", "ADMIN", "CUSTOM"] },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("roles", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["roleId", "roleName", "enterpriseId", "unixTimestampCreated"],
      properties: {
        roleId: { bsonType: "string", maxLength: 64 },
        roleName: { bsonType: "string", maxLength: 64 },
        description: { bsonType: "string", maxLength: 256 },
        enterpriseId: { bsonType: "string", maxLength: 64 },
        permissions: {
          bsonType: "array",
          maxItems: 1024,
          items: {
            bsonType: "object",
            required: ["applicationPermissionId"],
            properties: {
              applicationPermissionId: { bsonType: "string", maxLength: 64 },
              unixTimestampAdded: { bsonType: "long" },
              isActive: { bsonType: "bool" }
            }
          }
        },
        permissionsCount: { bsonType: "int", minimum: 0, maximum: 1024 },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("userRoleMappings", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["userRoleMappingId", "userId", "roleId", "unixTimestampCreated"],
      properties: {
        userRoleMappingId: { bsonType: "string", maxLength: 64 },
        userId: { bsonType: "string", maxLength: 64 },
        roleId: { bsonType: "string", maxLength: 64 },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampUpdated: { bsonType: "long" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("loginSessions", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["sessionId", "userId", "unixTimestampCreated", "unixTimestampExpires"],
      properties: {
        sessionId: { bsonType: "string", maxLength: 64 },
        userId: { bsonType: "string", maxLength: 64 },
        unixTimestampCreated: { bsonType: "long" },
        unixTimestampExpires: { bsonType: "long" },
        location: {
          bsonType: "object",
          properties: {
            latitude: { bsonType: "double" },
            longitude: { bsonType: "double" },
            altitude: { bsonType: "double" }
          }
        },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("telemetryData", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["telemetryDataId", "deviceId", "unixTimestamp", "dataType"],
      properties: {
        telemetryDataId: { bsonType: "string", maxLength: 64 },
        deviceId: { bsonType: "string", maxLength: 64 },
        unixTimestamp: { bsonType: "long" },
        dataType: { enum: ["S", "N", "B", "L"] }, // String, Number, Boolean, Location
        valueString: { bsonType: "string", maxLength: 256 },
        valueNumber: { bsonType: "double" },
        valueBoolean: { bsonType: "bool" },
        valueLocation: {
          bsonType: "object",
          properties: {
            latitude: { bsonType: "double" },
            longitude: { bsonType: "double" },
            altitude: { bsonType: "double" }
          }
        },
        unit: { bsonType: "string", maxLength: 32 },
        accuracy: { bsonType: "double" },
        precision: { bsonType: "double" },
        rangeMin: { bsonType: "double" },
        rangeMax: { bsonType: "double" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("alarms", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["alarmId", "deviceId", "unixTimestamp", "alarmType"],
      properties: {
        alarmId: { bsonType: "string", maxLength: 64 },
        deviceId: { bsonType: "string", maxLength: 64 },
        unixTimestamp: { bsonType: "long" },
        alarmType: { enum: ["C", "W", "I"] }, // Critical, Warning, Info
        description: { bsonType: "string", maxLength: 256 },
        relatedTelemetry: {
          bsonType: "array",
          items: {
            bsonType: "object",
            required: ["telemetryDataId"],
            properties: {
              telemetryDataId: { bsonType: "string", maxLength: 64 },
              mappingIndex: { bsonType: "int", minimum: 0 },
              unixTimestampAdded: { bsonType: "long" }
            }
          }
        },
        relatedTelemetryCount: { bsonType: "int", minimum: 0 },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

db.createCollection("notifications", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["notificationId", "userId", "unixTimestamp", "notificationType"],
      properties: {
        notificationId: { bsonType: "string", maxLength: 64 },
        userId: { bsonType: "string", maxLength: 64 },
        unixTimestamp: { bsonType: "long" },
        notificationType: { enum: ["A", "T", "E"] }, // Alarm, Telemetry, Event
        description: { bsonType: "string", maxLength: 256 },
        isRead: { bsonType: "bool" },
        isActive: { bsonType: "bool" },
        isSystem: { bsonType: "bool" }
      }
    }
  }
});

// Create indexes for optimal performance
print("Creating indexes...");

// Enterprise and User indexes
db.enterprises.createIndex({ "enterpriseId": 1 }, { unique: true });
db.users.createIndex({ "userId": 1 }, { unique: true });
db.users.createIndex({ "enterpriseId": 1, "userName": 1 }, { unique: true });
db.users.createIndex({ "enterpriseId": 1, "email": 1 }, { unique: true });
db.users.createIndex({ "email": 1 });

// Category indexes
db.categories.createIndex({ "categoryId": 1 }, { unique: true });
db.subcategories.createIndex({ "subcategoryId": 1 }, { unique: true });
db.subcategories.createIndex({ "categoryId": 1 });

// Site and spatial indexes
db.sites.createIndex({ "siteId": 1 }, { unique: true });
db.sites.createIndex({ "enterpriseId": 1 });
db.levels.createIndex({ "levelId": 1 }, { unique: true });
db.levels.createIndex({ "siteId": 1 });
db.levels.createIndex({ "siteId": 1, "levelNumber": 1 }, { unique: true });
db.zones.createIndex({ "zoneId": 1 }, { unique: true });
db.zones.createIndex({ "levelId": 1 });

// Geospatial indexes for location-based queries
db.zones.createIndex({ "zonePoints": "2dsphere" });
db.levels.createIndex({ "bounds.areaPoints": "2dsphere" });

// Device and Asset indexes
db.devices.createIndex({ "deviceId": 1 }, { unique: true });
db.devices.createIndex({ "enterpriseId": 1 });
db.devices.createIndex({ "deviceType": 1 });
db.devices.createIndex({ "deviceSubType": 
