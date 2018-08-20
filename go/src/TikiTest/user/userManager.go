package user

import (
	"fmt"
	"log"
)

const (
	UNREGISTER = "UNREGISTER"
	REGISTER   = "REGISTER"
	SILVER     = "SILVER"
	GOLD       = "GOLD"
)

type user struct {
	Name, Email, Group string
	ShoppingCartUUID   string
}

type UserManager struct {
	users map[string][]*user
}

func (um *UserManager) Init() {
	um.users = make(map[string][]*user)
	um.users[UNREGISTER] = make([]*user, 0)
	um.users[REGISTER] = make([]*user, 0)
	um.users[SILVER] = make([]*user, 0)
	um.users[GOLD] = make([]*user, 0)
}

func (um *UserManager) CreateUser(name, email, group, shoppingCartUUID string) error {
	if _, ok := um.users[group]; ok {
		for _, u := range um.users[group] {
			if u.Email == email {
				return fmt.Errorf("(CreateUser) failed to create %s %s %s, user existed", name, email, group)
			}
		}

		newUser := new(user)
		newUser.Email = email
		newUser.Group = group
		newUser.Name = name
		newUser.ShoppingCartUUID = shoppingCartUUID

		um.users[group] = append(um.users[group], newUser)
		log.Println("DEBUG: (CreateUser) created user", name, email, group)
		return nil
	}
	return fmt.Errorf("(CreateUser) failed to create %s %s %s, %s not found", name, email, group, group)
}

func (um *UserManager) GetUser(name, email, group string) *user {
	if _, ok := um.users[group]; ok {
		for _, u := range um.users[group] {
			if u.Name == name && u.Email == email {
				return u
			}
		}
	}
	return nil
}

func (um *UserManager) GetAllUsersInGroup(group string) []*user {
	if _, ok := um.users[group]; ok {
		return um.users[group]
	}
	return nil
}
