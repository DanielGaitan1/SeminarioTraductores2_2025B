// validador.js

// 1. Importamos la librería
const Ajv = require("ajv");
const ajv = new Ajv({ allErrors: true });

// 2. Cargamos tus archivos
const esquema = require("./schema.json");
const loteDeOrdenes = require("./datos.json");

// 3. Compilamos el validador
const validar = ajv.compile(esquema);

console.log("=== SISTEMA DE VALIDACIÓN AUTOCRISTALES GAITAN ===");
console.log("==================================================");

// 4. Recorremos cada orden
loteDeOrdenes.forEach((orden) => {
    console.log(`\n🆔 ID ORDEN: ${orden.id}`);
    
    // --- ESTA ES LA PARTE NUEVA ---
    // Imprimimos los datos que vamos a analizar para que puedas comparar visualmente
    console.log("📄 DATOS RECIBIDOS:");
    console.log(JSON.stringify(orden.datos, null, 2)); 
    console.log("-----------------------------------");

    const esValido = validar(orden.datos);

    if (esValido) {
        console.log("✅ ESTADO: APROBADO. La orden pasa al taller.");
    } else {
        console.log("❌ ESTADO: RECHAZADO. Errores encontrados:");
        validar.errors.forEach((error) => {
            console.log(`   - Campo '${error.instancePath}': ${error.message}`);
        });
    }
    console.log("__________________________________________________");
});