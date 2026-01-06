# Watch

Auto-compilador que detecta cambios y recompila automáticamente.

## Características

- Detecta cambios en archivos C/C++ automáticamente
- Recompila instantáneamente al guardar
- Muestra errores en tiempo real
- Super ligero (menos de 200 líneas de C)
- Sin configuración necesaria
- Escanea subdirectorios automáticamente

## Instalación

```bash
gcc watch.c -o watch
```

## Uso básico

```bash
# Auto-compilar con gcc
./watch gcc main.c -o app

# Con múltiples archivos
./watch gcc main.c util.c lib.c -o programa

# Usar con blaze
./watch ./blaze

# Usar con make
./watch make

# Sin limpiar pantalla entre compilaciones
./watch -n gcc test.c
```

## Comandos

- `watch <comando>` - Ejecuta comando cada vez que detecta cambios
- `watch -n <comando>` - Igual pero sin limpiar pantalla

## Ejemplo de flujo de trabajo

```bash
# Terminal 1: Deja watch corriendo
$ ./watch gcc main.c util.c -o app
Observando 15 archivos en el directorio actual
Comando: gcc main.c util.c -o app

═══════════════════════════════════════════════
⟳ Compilando... [Mon Jan 05 15:30:00 2026]
═══════════════════════════════════════════════

✓ Éxito
═══════════════════════════════════════════════
Observando cambios...

# Terminal 2: Edita tu código
$ vim main.c
# Guardas el archivo...

# Terminal 1: Watch detecta y recompila automáticamente!
→ Cambio detectado!

═══════════════════════════════════════════════
⟳ Compilando... [Mon Jan 05 15:30:15 2026]
═══════════════════════════════════════════════

✓ Éxito
═══════════════════════════════════════════════
Observando cambios...

# Terminal 3: Prueba tu programa
$ ./app
```

## Archivos que detecta

Watch automáticamente observa estos tipos de archivo:
- `.c` - Código C
- `.h` - Headers C
- `.cpp` - Código C++
- `.hpp` - Headers C++
- `.cc` - Código C++

Escanea el directorio actual y todos los subdirectorios.

## Cómo funciona

1. Escanea todos los archivos relevantes al iniciar
2. Guarda el timestamp de modificación de cada uno
3. Cada segundo verifica si algún archivo cambió
4. Si detecta cambio, ejecuta el comando de compilación
5. Muestra el resultado (éxito o error)
6. Vuelve a observar

## Ventajas

- **Productividad**: No tienes que cambiar de ventana para compilar
- **Feedback inmediato**: Ves errores apenas guardas
- **Simple**: Un solo comando y ya está funcionando
- **Flexible**: Funciona con cualquier comando (gcc, make, blaze, etc)

## Casos de uso

**Desarrollo iterativo:**
```bash
./watch gcc -Wall -g main.c -o debug
# Editas, guardas, ves errores, corriges, repites
```

**Con sistema de construcción:**
```bash
./watch ./blaze
# O con make
./watch make
```

**Testing continuo:**
```bash
./watch "gcc test.c -o test && ./test"
# Compila y ejecuta tests automáticamente
```

**Múltiples comandos:**
```bash
./watch "gcc main.c -o app && ./app"
# Compila y ejecuta si la compilación fue exitosa
```

## Opciones

- Sin opciones: Limpia pantalla entre compilaciones
- `-n`: No limpia pantalla (útil para ver historial)

## Limitaciones

- Máximo 128 archivos observados
- Verifica cambios cada 1 segundo
- Solo detecta archivos C/C++ automáticamente

## Combinación con otros tools

```bash
# Con snap para guardar antes de cambios grandes
./snap save "antes de refactor"
./watch gcc main.c -o app
# Haces cambios experimentales con feedback inmediato
```

## Licencia

Dominio público. Úsalo como quieras.