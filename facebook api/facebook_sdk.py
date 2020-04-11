import json
import facebook
 
ACCESS_TOKEN = "EAAIBg2Cpo5YBAKUjV86FXX2ZCditop8l7XXHbE4EQoqA0WaLv4UgJ7FXuz5hOVkZB5TYE1GlZCZBgPZClh0nz0rGxRlh1Cj7L5J2WZC3XYPZCmXHK1tSPJesSsZC33DUIcZBYUZAFSj4r5W4zEsKHDYuia6s4y9GLyHiabm5i15xdtI6F0zq6wFZBZAHsDZC2SAoNFdEN8osm2dsEJwZDZD"
 
def main():
    token = ACCESS_TOKEN
    graph = facebook.GraphAPI(token)
    profile = graph.get_object('me',fields='first_name, id, like, posts')

    print("Printing profile: ")
    print(type(profile))
    print(json.dumps(profile, indent=4))
    print(type(json.dumps(profile,indent=4)))


    