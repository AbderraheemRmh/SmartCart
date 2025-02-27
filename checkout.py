import serial

# Change this to your Arduino port (e.g., COM3 for Windows, /dev/ttyUSB0 for Linux/Mac)
arduino_port = "COM5"
baud_rate = 9600  

try:
    arduino = serial.Serial(arduino_port, baud_rate, timeout=1)
    print("Connected to RFID Scanner on", arduino_port)
except:
    print("Failed to connect to Arduino")
    exit()

#Product Database (RFID UID → Product Name & Price)
product_db = {
    "5326bbfd": ("Tea", 40),
    "3af4206f": ("Coffee", 60),
    "faa02f6f": ("Milk", 90),
}

cart = []  # Shopping cart (stores scanned products)
total_price = 0  # Total cost

print("\n🔹 Scan products using RFID")
print("🔹 Type 'checkout' to finalize the purchase\n")

while True:
    try:
        data = arduino.readline().decode("utf-8").strip()

        if data:
            if data in product_db:
                product_name, price = product_db[data]
                cart.append((product_name, price))
                total_price += price

                print(f"✅ {product_name} added to cart! Price: {price} DA")
                print(f"💰 Total: {total_price} DA")

                # Send product name & total price to Arduino (LCD Display)
                response = f"{product_name},{total_price}\n"
                arduino.write(response.encode("utf-8"))

            else:
                print(f"❌ Unknown RFID: {data}")
                arduino.write("Unknown,0\n".encode("utf-8"))  # Send default response

    except KeyboardInterrupt:
        print("\nExiting...")
        arduino.close()
        break

    # Check if user wants to checkout
    user_input = input("\nType 'checkout' to pay, or press Enter to continue: ").strip().lower()
    if user_input == "checkout":
        if cart:
            print("\n🛒 Shopping Cart Summary:")
            for item, price in cart:
                print(f"  - {item}: {price} DA")

            print(f"\n💰 Total: {total_price} DA")
            print("✅ Thank you for shopping!\n")
            cart.clear()  # Empty the cart after checkout
              

            # Send reset message to Arduino
            arduino.write(f"Total,{total_price}\n".encode("utf-8"))
            total_price = 0   
            # Reset total price

        else:
            print("⚠️ Your cart is empty!")
