package engine

import (
	"TikiTest/product"
	"TikiTest/shoppingCart"
	"TikiTest/user"
	"fmt"
)

type Engine struct {
	productManger       *product.ProductManager
	userManager         *user.UserManager
	shoppingCartManager *shoppingcart.ShoppingCartManager
}

func (e *Engine) Init() {
	e.productManger = new(product.ProductManager)
	e.productManger.Init()

	e.userManager = new(user.UserManager)
	e.userManager.Init()

	e.shoppingCartManager = new(shoppingcart.ShoppingCartManager)
	e.shoppingCartManager.Init()
}

func (e *Engine) AddNewProduct(name, color, symbol string, price float64, quantity int64) {
	e.productManger.AddProduct(name, color, symbol, price, quantity)
}

func (e *Engine) CreateUser(name, email, group string) error {
	shoppingCartUUID := e.shoppingCartManager.CreateShoppingCartUUID()
	err := e.userManager.CreateUser(name, email, group, shoppingCartUUID)
	if err != nil {
		e.shoppingCartManager.RemoveShoppingCart(shoppingCartUUID)
		return err
	}
	return nil
}

func (e *Engine) Add2Cart(name, email, group, productName, productColor string, quatity int64) error {
	if quatity <= 0 {
		return nil
	}
	user := e.userManager.GetUser(name, email, group)
	if user == nil {
		return fmt.Errorf("(Add2Cart) failed to get user %s %s %s", name, email, group)
	}
	product := e.productManger.GetProduct(productName, productColor)
	if product == nil {
		return fmt.Errorf("(Add2Cart) failed to get product %s %s", productName, productColor)
	}
	shoppingcart := e.shoppingCartManager.GetShoppingCart(user.ShoppingCartUUID)
	if shoppingcart == nil {
		return fmt.Errorf("(Add2Cart) failed to get shopping cart %s %s %s", name, email, group)
	}
	err := shoppingcart.Add(product, quatity)
	if err != nil {
		return err
	}
	// spew.Println(e.userManager)
	// spew.Println(e.productManger)
	// spew.Println(e.shoppingCartManager)
	return nil
}

func (e *Engine) GetTotalPrice(name, email, group string) (float64, string, error) {
	user := e.userManager.GetUser(name, email, group)
	if user == nil {
		return 0, "", fmt.Errorf("(GetTotalPrice) failed to get user %s %s %s", name, email, group)
	}
	shoppingcart := e.shoppingCartManager.GetShoppingCart(user.ShoppingCartUUID)
	if shoppingcart == nil {
		return 0, "", fmt.Errorf("(GetTotalPrice) failed to get shopping cart %s %s %s", name, email, group)
	}

	totalPrice, symbol := shoppingcart.CalculateTotalPrice(e.productManger)
	return totalPrice, symbol, nil
}

func (e *Engine) ApplyShoppingCartRule(rule shoppingcart.ShoppingCartRule) {
	e.shoppingCartManager.ApplyShoppingCartRule(rule, e.productManger, e.userManager)
}
