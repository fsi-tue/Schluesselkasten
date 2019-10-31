#!/usr/bin/python3
# coding=utf-8
import requests
from api_constants import *

"""
format for the api_constants.py file:
API_URL = <api-url-here>
API_KEY = <api-key-here>
HEADER = {"Content-type": "application/x-www-form-urlencoded",
          "Accept": "text/plain"}
"""


def reverse_octet(string):
    """
    The ID coming from the reader is truncated to its first 10 digits and cast to HEX
    The octets of this HEX number are reversed and the "0x" is stripped in order to
    obtain the actual UID of the card.
    :param string: the HEX string converted from DEC (coming from the reader)
    :return: the octets in reversed order (this corresponds to the actual UID of the card)
    """
    return "" if not string else reverse_octet(string[2:]) + string[:2]


def call_create(url, card_id, member_name):
    """
    Calls the API to insert a new member into the database.
    :param url: The URL of the API to be called
    :param card_id: the UID of the card to be added (has to be preprocessed first)
    :param member_name: The full name of the member
    :return: the response of the API
    """
    post_data = {
        'apikey': API_KEY,
        'create': card_id + ";" + member_name
    }
    return requests.post(url, data=post_data, headers=HEADER)


def call_getall(url):
    """
    Calls the API to get all members currently listed in the database.
    :param url:
    :return: A list of all members in the format
    [ (uid_i, name_i), (uid_i+1, name_i+1), (uid_n, name_n)]
    """


def call_remove(url, uid):
    """
    Removes a member from the database using the UID of the card.
    :param url:
    :param uid:
    :return:
    """


def add_member():
    """
    Adds a member to the database.
    The name of the member and the card "ID" are read via console input.
    The ID from the reader is processed via reverse_octet. This info is then sent to the API via call_create().
    """

    print("=> Nutzer hinzufügen")
    # get full name of member
    name = input("Bitte vollständigen Namen eingeben:")

    # read decimal ID
    id_dec = int(input("Bitte Studentenausweis auflegen"))
    hex_reversed_truncated = reverse_octet(hex(id_dec))[:-2].lower()

    print("\nFolgende Daten werden gesendet:\n" + hex_reversed_truncated + '\n' + name)

    print("Sende...")
    res = call_create(API_URL, hex_reversed_truncated, name)
    if "Created" in res.text:
        print("Eintrag erfolgreich angelegt!\n")
    else:
        print("Fehler beim Anlegen: ", res.text, '\n')


def remove_member():

    """
    Removes a member from the database. This is done by first fetching all members from the database
    and then selecting the member to be removed.
    :return:
    """

if __name__ == '__main__':
    print("""
    ===========================
    == Schlüsselkastensystem ==
    ===========================
    """)
    while True:
        add_member()
