# Define the dimensions and maximum value for the PPM file
width, height = 16, 16
maxval = 255

# Create the header for the PPM file
header = f"P6\n{width} {height}\n{maxval}\n"

# Create the pixel data
data = bytearray()
for y in range(height):
    for x in range(width):
        if (x + y) % 2 == 0:
            # White pixel
            data.extend([255, 255, 255])
        else:
            # Black pixel
            data.extend([0, 0, 0])

# Write the header and pixel data to the PPM file
with open("chessboard.ppm", "wb") as f:
    f.write(header.encode('ascii'))
    f.write(data)

