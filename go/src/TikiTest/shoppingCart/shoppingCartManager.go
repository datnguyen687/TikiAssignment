package shoppingcart

import (
	"TikiTest/product"
	"TikiTest/user"
	"fmt"
	"time"

	"github.com/satori/go.uuid"
)

type ShoppingCartRule struct {
	FromDate, ToDate string
	UserGroup        string
	Color            string
	SubTotal         float64
	Discount         float64
	Symbol           string
}

type productDetail struct {
	ID       string
	Quantity int64
}

type shoppingCart struct {
	UUID           string
	ProductDetails []*productDetail
	Rules          []*ShoppingCartRule
}

func (sc *shoppingCart) Add(product *product.Product, quantity int64) error {
	if product.Quantity < quantity {
		return fmt.Errorf("(Add) failed to add to shopping cart %s %s %d, order %d", product.Name, product.Color, product.Quantity, quantity)
	}
	product.Quantity -= quantity
	for _, prd := range sc.ProductDetails {
		if prd.ID == product.UUID {
			prd.Quantity += quantity
			return nil
		}
	}
	productDetail := new(productDetail)
	productDetail.ID = product.UUID
	productDetail.Quantity = quantity
	sc.ProductDetails = append(sc.ProductDetails, productDetail)
	return nil
}

func (sc *shoppingCart) Remove(product *product.Product, quantity int64) error {
	for i, prd := range sc.ProductDetails {
		if prd.ID == product.UUID {
			product.Quantity += quantity
			sc.ProductDetails = append(sc.ProductDetails[:i], sc.ProductDetails[i+1:]...)
			return nil
		}
	}
	return fmt.Errorf("(Remove) not found in cart %s %s", product.Name, product.Color)
}

func (sc *shoppingCart) CalculateTotalPrice(pm *product.ProductManager) (float64, string) {
	sum := float64(0)
	for _, pid := range sc.ProductDetails {
		product := pm.GetProductByID(pid.ID)
		sum += float64(pid.Quantity) * (product.Price)
	}
	discount := float64(0)
	for _, rule := range sc.Rules {
		discount += rule.Discount
	}
	sum -= discount
	return sum, "$"
}

type ShoppingCartManager struct {
	ShoppingCarts map[string]*shoppingCart
}

func (scm *ShoppingCartManager) Init() {
	scm.ShoppingCarts = make(map[string]*shoppingCart)
}

func (scm *ShoppingCartManager) CreateShoppingCartUUID() string {
	uuid := uuid.Must(uuid.NewV4()).String()
	sc := new(shoppingCart)
	sc.UUID = uuid
	sc.ProductDetails = make([]*productDetail, 0)
	sc.Rules = make([]*ShoppingCartRule, 0)
	scm.ShoppingCarts[uuid] = sc
	return uuid
}

func (scm *ShoppingCartManager) RemoveShoppingCart(uuid string) {
	if _, ok := scm.ShoppingCarts[uuid]; ok {
		delete(scm.ShoppingCarts, uuid)
	}
}

func (scm *ShoppingCartManager) GetShoppingCart(uuid string) *shoppingCart {
	if sc, ok := scm.ShoppingCarts[uuid]; ok {
		return sc
	}
	return nil
}

func (scm *ShoppingCartManager) ApplyShoppingCartRule(rule ShoppingCartRule, product *product.ProductManager, user *user.UserManager) {
	fromDate, _ := time.Parse("20060102", rule.FromDate)
	toDate, _ := time.Parse("20060102", rule.ToDate)
	today := time.Now().Local()
	if today.Before(fromDate) || today.After(toDate) {
		return
	}
	allUsersInGroup := user.GetAllUsersInGroup(rule.UserGroup)
	if allUsersInGroup != nil {
		for _, usr := range allUsersInGroup {
			cart := scm.GetShoppingCart(usr.ShoppingCartUUID)
			totalPrice := float64(0)
			for _, pd := range cart.ProductDetails {
				p := product.GetProductByID(pd.ID)
				if p.Color == rule.Color {
					totalPrice += float64(pd.Quantity) * (p.Price)
				}
			}
			if totalPrice >= rule.SubTotal {
				cart.Rules = append(cart.Rules, &rule)
			}
		}
	}
	//spew.Println(scm.ShoppingCarts)
}
