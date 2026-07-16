import base64

flag = "V29qxDi0b3Y5aDesyi90VUYmU2meWDicWUeiMw8/cN=="

custom_alphabet = "ZYXABCDEFGHIJKLMNOPQRSTUVWzyxabcdefghijklmnopqrstuvw0123456789+/"
standard_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

translation_table_decode = str.maketrans(custom_alphabet, standard_alphabet)
standard_b64_decode = flag.translate(translation_table_decode)

decoded_data = base64.b64decode(standard_b64_decode)

username = input("enter username: ")
password = input("enter password: ")

translation_table_encode = str.maketrans(standard_alphabet, custom_alphabet)
user_b64 = base64.b64encode(username.encode())
pass_b64 = base64.b64encode(password.encode())

username_enc = user_b64.decode().translate(translation_table_encode)
password_enc = pass_b64.decode().translate(translation_table_encode)

if username_enc == "aDSoyj9py2amzT5jy3GqVUKm" and password_enc == "V29qxDi0JgZvKN==":
  print(decoded_data.decode())
else:
  print("pertama tama, coba \"strings\" binarynya. abis itu coba cek apa yang menarik dari hasil strings itu, terus cek google atau tanya AI")

