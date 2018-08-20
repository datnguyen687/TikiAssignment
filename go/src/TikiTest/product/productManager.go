package product

import (
	"log"

	"github.com/satori/go.uuid"
)

type Product struct {
	Name, Color, UUID, Symbol string
	Price                     float64
	Quantity                  int64
}

type ProductManager struct {
	products map[string][]*Product
}

func (pm *ProductManager) Init() {
	pm.products = make(map[string][]*Product)
}

func (pm *ProductManager) AddProduct(name, color, symbol string, price float64, quantity int64) {
	if quantity <= 0 {
		return
	}
	found := false
	if val, ok := pm.products[name]; ok {
		for _, v := range val {
			if v.Color == color {
				log.Println("(AddProduct) Product %s %s %s%f already existed, increase the quantity %d", name, color, symbol, price, quantity)
				v.Quantity += quantity
			}
		}
		found = true
	}

	if !found {
		pm.products[name] = make([]*Product, 0)
	}
	p := new(Product)
	p.Color = color
	p.Name = name
	p.Price = price
	p.UUID = uuid.Must(uuid.NewV4()).String()
	p.Symbol = symbol
	p.Quantity = quantity

	pm.products[name] = append(pm.products[name], p)
	log.Println("DEBUG: (AddProduct) add new item", name, color, symbol, price, quantity)
}

func (pm *ProductManager) GetProduct(productName, productColor string) *Product {
	if pr, ok := pm.products[productName]; ok {
		for _, p := range pr {
			if p.Name == productName && p.Color == productColor {
				return p
			}
		}
	}
	return nil
}

func (pm *ProductManager) GetProductByID(uuid string) *Product {
	for _, val := range pm.products {
		for _, p := range val {
			if p.UUID == uuid {
				return p
			}
		}
	}
	return nil
}
