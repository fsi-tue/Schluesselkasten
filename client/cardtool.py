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
        'action': 'create',
        'usr': card_id + ";" + member_name
    }
    return requests.post(url, data=post_data, headers=HEADER)


def call_getall(url):
    """
    Calls the API to get all members currently listed in the database.
    :param url:
    :return: A list of all members in the format
    [ [uid_i, name_i], [uid_i+1, name_i+1], [uid_n, name_n]]
    """
    post_data = {
        'apikey': API_KEY,
        'action': 'getall'
    }
    return requests.post(url, data=post_data, headers=HEADER)


def call_remove(url, uid, member_name):
    """
    Removes a member from the database using the UID of the card.
    :param url:
    :param uid:
    :param member_name:
    :return:
    """
    post_data = {
        'apikey': API_KEY,
        'action': 'delete',
        'usr': uid + ';' + member_name
    }
    return requests.post(url, data=post_data, headers=HEADER)


def add_member():
    """
    Adds a member to the database.
    The name of the member and the card "ID" are read via console input.
    The ID from the reader is processed via reverse_octet. This info is then sent to the API via call_create().
    """

    print("=> Nutzer hinzufügen")
    # get full name of member
    name = input("Bitte vollständigen Namen eingeben:\n")

    # read decimal ID
    id_dec = int(input("Bitte Studentenausweis auflegen\n"))
    hex_reversed_truncated = reverse_octet(hex(id_dec))[:-2].lower()

    # Check if the member is already present in database
    list_allmembers = get_all_members()

    if hex_reversed_truncated not in list_allmembers:
        print("\nFolgende Daten werden gesendet:\n" + hex_reversed_truncated + '\n' + name)

        print("Sende...")
        res = call_create(API_URL, hex_reversed_truncated, name)
        if "Created" in res.text:
            print("Eintrag erfolgreich angelegt!\n")
        else:
            print("Fehler beim Anlegen: ", res.text, '\n')
    else:
        print("Eintrag ist bereits vorhanden!")


def remove_member():
    """
    Removes a member from the database. This is done by first fetching all members from the database
    and then selecting the member to be removed.
    :return:
    """
    list_allmembers = get_all_members()
    list_allmembers = list_allmembers[1:-1]  # truncate empty line at end of file and first line
    length = len(list_allmembers)

    for i in range(length):
        # i[0]: UID of member
        # i[1]: Name of member
        print(str(i) + ') ' + str(list_allmembers[i][1]))

    print("=> Nutzer löschen")
    member = int(input("Bitte Zahl eingeben, die gelöscht werden soll\n"))
    member_uid = list_allmembers[member][0]
    member_name = list_allmembers[member][1]
    print("GELÖSCHT wird folgender Eintrag:")
    print("Name: " + member_name + "\n" + "UID: " + member_uid)
    input("Enter zum Bestätigen, Strg-C für Abbruch.\n")

    res = call_remove(API_URL, member_uid, member_name)
    if "Deleted" in res.text:
        print("Eintrag erfolgreich gelöscht!\n")
    else:
        print("Fehler beim Löschen: ", res.text, '\n')


def get_all_members():
    """"
    Returns a list of all UIDs present in the dataset.
    This is done by calling the API for a list of all items and then 
    piping them into a list.
    """
    res = call_getall(API_URL)
    response = res.text
    all_members = response.split('\n')
    all_members = [i.split(';') for i in all_members]

    return all_members


if __name__ == '__main__':
    while True:
        print("""
        ===========================
        == Schlüsselkastensystem ==
        ===========================
        """)
        print("""
            1) Mitglied(er) hinzufügen
            2) Mitglied löschen
        """)
        choice = int(input("Bitte wählen."))
        if choice == 1:
            add_member()
        if choice == 2:
            remove_member()
