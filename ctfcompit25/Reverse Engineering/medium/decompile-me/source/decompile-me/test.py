import base64

flag = "compit{python_tapi_jadi_exe???}"
username = "teknologiinformasi"
password = "compit2025"

custom_alphabet = "ZYXABCDEFGHIJKLMNOPQRSTUVWzyxabcdefghijklmnopqrstuvw0123456789+/"
standard_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

translation_table_encode = str.maketrans(standard_alphabet, custom_alphabet)
flag_b64 = base64.b64encode(flag.encode())
user_b64 = base64.b64encode(username.encode())
pass_b64 = base64.b64encode(password.encode())

flag_enc = flag_b64.decode().translate(translation_table_encode)
username_enc = user_b64.decode().translate(translation_table_encode)
password_enc = pass_b64.decode().translate(translation_table_encode)

print(flag_enc)
print(username_enc)
print(password_enc)