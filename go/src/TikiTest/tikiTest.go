package main

import (
	"TikiTest/engine"
	"TikiTest/shoppingCart"
	"TikiTest/user"
	"fmt"
	"log"
)

func main() {
	engine := new(engine.Engine)
	engine.Init()
	engine.AddNewProduct("Iphone", "Silver", "$", 999, 2)
	engine.AddNewProduct("Iphone", "Black", "$", 899, 1)

	err := engine.CreateUser("John Doe 1", "john.doe@example.com", user.GOLD)
	if err != nil {
		log.Println("ERROR:", err)
		return
	}

	err = engine.Add2Cart("John Doe 1", "john.doe@example.com", user.GOLD, "Iphone", "Silver", 1)
	if err != nil {
		log.Println("ERROR:", err)
		return
	}

	err = engine.Add2Cart("John Doe 1", "john.doe@example.com", user.GOLD, "Iphone", "Silver", 1)
	if err != nil {
		log.Println("ERROR:", err)
		return
	}

	err = engine.Add2Cart("John Doe 1", "john.doe@example.com", user.GOLD, "Iphone", "Black", 1)
	if err != nil {
		log.Println("ERROR:", err)
		return
	}

	totalPrice, symbol, err := engine.GetTotalPrice("John Doe 1", "john.doe@example.com", user.GOLD)
	if err != nil {
		log.Println("ERROR:", err)
		return
	}
	fmt.Printf("%s%f\n", symbol, totalPrice)

	engine.ApplyShoppingCartRule(shoppingcart.ShoppingCartRule{
		Color:     "Black",
		Discount:  50,
		FromDate:  "20180814",
		SubTotal:  150,
		Symbol:    "$",
		ToDate:    "20180816",
		UserGroup: user.GOLD,
	})

	totalPrice, symbol, err = engine.GetTotalPrice("John Doe 1", "john.doe@example.com", user.GOLD)
	if err != nil {
		log.Println("ERROR:", err)
		return
	}
	fmt.Printf("%s%f\n", symbol, totalPrice)
}
