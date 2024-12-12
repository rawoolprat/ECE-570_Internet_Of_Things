import paho.mqtt.client as mqtt
import mysql.connector
from datetime import datetime

# MQTT Broker Configuration
BROKER_URL = "1bd4a0df136c4f1592d4e1c2b9963a12.s1.eu.hivemq.cloud"
BROKER_PORT = 8884
MQTT_USER = "rawprat"
MQTT_PASSWORD = "Frozen8642"
TOPIC = "testtopic/temp/outTopic/potvalue"

# Database Configuration
HOST = "srv1567.hstgr.io"
USER = "u411807168_pratiksha"
PASSWORD = "Biotin@282019"
DATABASE = "u411807168_IOT"

# Global variable for database connection
db_connection = None

# Set up the database connection
def setup_database_connection():
    global db_connection
    try:
        db_connection = mysql.connector.connect(
            host=HOST,
            user=USER,
            password=PASSWORD,
            database=DATABASE
        )
        if db_connection.is_connected():
            print("Database connection established.")
    except mysql.connector.Error as err:
        print(f"Database Error: {err}")

# Push data to the database
def push_value_to_db(sensor_value):
    global db_connection
    try:
        # Ensure the connection is still active
        if not db_connection or not db_connection.is_connected():
            setup_database_connection()

        cursor = db_connection.cursor()
        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        insert_query = "INSERT INTO sensor_value (value, timestamp) VALUES (%s, %s)"
        cursor.execute(insert_query, (sensor_value, timestamp))
        db_connection.commit()
        print(f"Value {sensor_value} inserted into database with timestamp {timestamp}!")
        cursor.close()
    except mysql.connector.Error as err:
        print(f"Database Error: {err}")

# Close the database connection
def close_database_connection():
    global db_connection
    if db_connection and db_connection.is_connected():
        db_connection.close()
        print("Database connection closed.")

# MQTT Callbacks
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to HiveMQ broker!")
        client.subscribe(TOPIC)
        print(f"Subscribed to topic: {TOPIC}")
    else:
        print(f"Failed to connect to broker. Return code {rc}")

def on_message(client, userdata, msg):
    try:
        # Print received message
        message = msg.payload.decode()
        print(f"Received message: {message} from topic: {msg.topic}")

        # Check if the message contains voltage data
        if "V" in message:
            voltage = float(message.split()[0])  # Extract voltage value
            print(f"Extracted Voltage: {voltage} V")

            # Push the voltage value to the database
            push_value_to_db(voltage)
        else:
            print(f"Invalid message format: {message}")

    except Exception as e:
        print(f"Error processing message: {e}")

# Main program setup
setup_database_connection()  # Establish the initial database connection

client = mqtt.Client(transport="websockets")
client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
client.tls_set()  # Enable secure connection
client.on_connect = on_connect
client.on_message = on_message

# Connect to the MQTT broker
print("Connecting to MQTT broker...")
client.connect(BROKER_URL, BROKER_PORT, 60)

try:
    client.loop_forever()  # Start the MQTT client loop
finally:
    close_database_connection()  # Ensure the database connection is closed properly when the program exits
